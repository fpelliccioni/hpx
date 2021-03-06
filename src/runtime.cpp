//  Copyright (c) 2007-2012 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>

#include <hpx/state.hpp>
#include <hpx/exception.hpp>
#include <hpx/include/runtime.hpp>
#include <hpx/runtime/components/runtime_support.hpp>
#include <hpx/runtime/threads/threadmanager.hpp>
#include <hpx/runtime/threads/policies/topology.hpp>
#include <hpx/include/performance_counters.hpp>
#include <hpx/runtime/agas/big_boot_barrier.hpp>
#include <hpx/util/high_resolution_clock.hpp>
#include <hpx/util/coroutine/detail/coroutine_impl_impl.hpp>
#include <hpx/util/backtrace.hpp>
#include <hpx/util/query_counters.hpp>

#include <iostream>
#include <vector>

#if defined(_WIN64) && defined(_DEBUG) && !defined(HPX_COROUTINE_USE_FIBERS)
#include <io.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Make sure the system gets properly shut down while handling Ctrl-C and other
// system signals
#if defined(BOOST_WINDOWS)

namespace hpx
{
    void handle_termination(char const* reason)
    {
        std::cerr
#if defined(HPX_HAVE_STACKTRACES)
            << "{stack-trace}: " << hpx::util::trace() << "\n"
#endif
            << "{what}: " << (reason ? reason : "Unknown reason") << "\n"
            << full_build_string();           // add full build information

        std::abort();
    }

    HPX_EXPORT BOOL WINAPI termination_handler(DWORD ctrl_type)
    {
        switch (ctrl_type) {
        case CTRL_C_EVENT:
            handle_termination("Ctrl-C");
            return TRUE;

        case CTRL_BREAK_EVENT:
            handle_termination("Ctrl-Break");
            return TRUE;

        case CTRL_CLOSE_EVENT:
            handle_termination("Ctrl-Close");
            return TRUE;

        case CTRL_LOGOFF_EVENT:
            handle_termination("Logoff");
            return TRUE;

        case CTRL_SHUTDOWN_EVENT:
            handle_termination("Shutdown");
            return TRUE;

        default:
            break;
        }
        return FALSE;
    }
}

#else

#include <signal.h>
#include <stdlib.h>
#include <string.h>

namespace hpx
{
    HPX_EXPORT void termination_handler(int signum)
    {
        char* reason = strsignal(signum);
        std::cerr
#if defined(HPX_HAVE_STACKTRACES)
            << "{stack-trace}: " << hpx::util::trace() << "\n"
#endif
            << "{what}: " << (reason ? reason : "Unknown signal") << "\n"
            << full_build_string();           // add full build information

        std::abort();
    }
}

#endif

///////////////////////////////////////////////////////////////////////////////
namespace hpx
{
    HPX_EXPORT void new_handler()
    {
        HPX_THROW_EXCEPTION(out_of_memory, "new_handler",
            "new allocator failed to allocate memory");
    }

    void set_error_handlers()
    {
#if defined(BOOST_WINDOWS)
        // Set console control handler to allow server to be stopped.
        SetConsoleCtrlHandler(hpx::termination_handler, TRUE);
#else
        struct sigaction new_action;
        new_action.sa_handler = hpx::termination_handler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;

        sigaction(SIGINT, &new_action, NULL);  // Interrupted
        sigaction(SIGBUS, &new_action, NULL);  // Bus error
        sigaction(SIGFPE, &new_action, NULL);  // Floating point exception
        sigaction(SIGILL, &new_action, NULL);  // Illegal instruction
        sigaction(SIGPIPE, &new_action, NULL); // Bad pipe
        sigaction(SIGSEGV, &new_action, NULL); // Segmentation fault
        sigaction(SIGSYS, &new_action, NULL);  // Bad syscall
#endif

        std::set_new_handler(hpx::new_handler);
    }


    ///////////////////////////////////////////////////////////////////////////
    namespace strings
    {
        char const* const runtime_mode_names[] =
        {
            "invalid",    // -1
            "console",    // 0
            "worker",     // 1
            "connect",    // 2
            "default",    // 3
        };
    }

    char const* get_runtime_mode_name(runtime_mode state)
    {
        if (state < runtime_mode_invalid || state >= runtime_mode_last)
            return "invalid (value out of bounds)";
        return strings::runtime_mode_names[state+1];
    }

    runtime_mode get_runtime_mode_from_name(std::string const& mode)
    {
        for (std::size_t i = 0; i < runtime_mode_last; ++i) {
            if (mode == strings::runtime_mode_names[i])
                return static_cast<runtime_mode>(i-1);
        }
        return runtime_mode_invalid;
    }

    ///////////////////////////////////////////////////////////////////////////
    runtime::runtime(util::runtime_configuration const& rtcfg)
      : ini_(rtcfg),
        instance_number_(++instance_number_counter_),
        topology_(threads::create_topology()),
        state_(state_invalid)
    {
        // initialize our TSS
        runtime::init_tss();

        counters_.reset(new performance_counters::registry());
    }

    ///////////////////////////////////////////////////////////////////////////
    boost::atomic<int> runtime::instance_number_counter_(-1);

    ///////////////////////////////////////////////////////////////////////////
    util::thread_specific_ptr<runtime *, runtime::tls_tag> runtime::runtime_;
    util::thread_specific_ptr<std::string, runtime::tls_tag> runtime::thread_name_;
    util::thread_specific_ptr<boost::uint64_t, runtime::tls_tag> runtime::uptime_;

    void runtime::init_tss()
    {
        // initialize our TSS
        if (NULL == runtime::runtime_.get())
        {
            BOOST_ASSERT(NULL == threads::coroutine_type::impl_type::get_self());

            runtime::runtime_.reset(new runtime* (this));
            runtime::uptime_.reset(new boost::uint64_t);
            *runtime::uptime_.get() = util::high_resolution_clock::now();

            threads::coroutine_type::impl_type::init_self();
        }
    }

    void runtime::deinit_tss()
    {
        // reset our TSS
        threads::coroutine_type::impl_type::reset_self();
        runtime::uptime_.reset();
        runtime::runtime_.reset();
    }

    std::string const* runtime::get_thread_name()
    {
        return runtime::thread_name_.get();
    }

    boost::uint64_t runtime::get_system_uptime()
    {
        boost::int64_t diff = util::high_resolution_clock::now() - *runtime::uptime_.get();
        return diff < 0LL ? 0ULL : static_cast<boost::uint64_t>(diff);
    }

    ///////////////////////////////////////////////////////////////////////////
    void runtime::start_active_counters(error_code& ec)
    {
        if (active_counters_.get())
            active_counters_->start_counters(ec);
    }

    void runtime::stop_active_counters(error_code& ec)
    {
        if (active_counters_.get())
            active_counters_->stop_counters(ec);
    }

    void runtime::reset_active_counters(error_code& ec)
    {
        if (active_counters_.get())
            active_counters_->reset_counters(ec);
    }

    void runtime::evaluate_active_counters(bool reset,
        char const* description, error_code& ec)
    {
        if (active_counters_.get())
            active_counters_->evaluate_counters(reset, description, ec);
    }

    parcelset::policies::message_handler* runtime::create_message_handler(
        char const* message_handler_type, char const* action,
        parcelset::parcelport* pp, std::size_t num_messages,
        std::size_t interval, error_code& ec)
    {
        return runtime_support_.create_message_handler(message_handler_type, 
            action, pp, num_messages, interval, ec);
    }

    util::binary_filter* runtime::create_binary_filter(
        char const* binary_filter_type, bool compress, error_code& ec)
    {
        return runtime_support_.create_binary_filter(binary_filter_type, 
            compress, ec);
    }

    /// \brief Register all performance counter types related to this runtime
    ///        instance
    void runtime::register_counter_types()
    {
        performance_counters::generic_counter_type_data statistic_counter_types[] =
        {
            // averaging counter
            { "/statistics/average", performance_counters::counter_aggregating,
              "returns the averaged value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/average",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::aggregating_counter_creator,
              &performance_counters::default_counter_discoverer,
              ""
            },

            // stddev counter
            { "/statistics/stddev", performance_counters::counter_aggregating,
              "returns the standard deviation value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/stddev",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::aggregating_counter_creator,
              &performance_counters::default_counter_discoverer,
              ""
            },

            // rolling_averaging counter
            { "/statistics/rolling_average", performance_counters::counter_aggregating,
              "returns the averaged value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/rolling_averaging",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::aggregating_counter_creator,
              &performance_counters::default_counter_discoverer,
              ""
            },

            // median counter
            { "/statistics/median", performance_counters::counter_aggregating,
              "returns the averaged value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/median",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::aggregating_counter_creator,
              &performance_counters::default_counter_discoverer,
              ""
            },

            // max counter
            { "/statistics/max", performance_counters::counter_aggregating,
              "returns the averaged value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/max",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::aggregating_counter_creator,
              &performance_counters::default_counter_discoverer,
              ""
            },

            // min counter
            { "/statistics/min", performance_counters::counter_aggregating,
              "returns the averaged value of its base counter over "
              "an arbitrary time line; pass required base counter as the instance "
              "name: /statistics{<base_counter_name>}/min",
              HPX_PERFORMANCE_COUNTER_V1,
               &performance_counters::detail::aggregating_counter_creator,
               &performance_counters::default_counter_discoverer,
              ""
            },

            // uptime counters
            { "/runtime/uptime", performance_counters::counter_elapsed_time,
              "returns the up time of the runtime instance for the referenced locality",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::uptime_counter_creator,
              &performance_counters::locality_counter_discoverer,
              "s"    // unit of measure is seconds
            },

            // component instance counters
            { "/runtime/count/component", performance_counters::counter_raw,
              "returns the number of component instances currently alive on "
              "this locality (the component type has to be specified as the "
              "counter parameter)",
              HPX_PERFORMANCE_COUNTER_V1,
              &performance_counters::detail::component_instance_counter_creator,
              &performance_counters::locality_counter_discoverer,
              ""
            }
        };
        performance_counters::install_counter_types(
            statistic_counter_types,
            sizeof(statistic_counter_types)/sizeof(statistic_counter_types[0]));
    }

    ///////////////////////////////////////////////////////////////////////////
    runtime& get_runtime()
    {
        BOOST_ASSERT(NULL != runtime::runtime_.get());   // should have been initialized
        return **runtime::runtime_;
    }

    runtime* get_runtime_ptr()
    {
        runtime** rt = runtime::runtime_.get();
        return rt ? *rt : NULL;
    }

    naming::locality const& get_locality()
    {
        return get_runtime().here();
    }

    void report_error(std::size_t num_thread, boost::exception_ptr const& e)
    {
        // Early and late exceptions
        if (!threads::threadmanager_is(running))
        {
            detail::report_exception_and_terminate(e);
            return;
        }

        hpx::applier::get_applier().get_thread_manager().report_error(num_thread, e);
    }

    void report_error(boost::exception_ptr const& e)
    {
        // Early and late exceptions
        if (!threads::threadmanager_is(running))
        {
            detail::report_exception_and_terminate(e);
            return;
        }

        std::size_t num_thread = hpx::threads::threadmanager_base::get_worker_thread_num();
        hpx::applier::get_applier().get_thread_manager().report_error(num_thread, e);
    }

    bool register_on_exit(HPX_STD_FUNCTION<void()> f)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL == rt)
            return false;

        rt->on_exit(f);
        return true;
    }

    std::size_t get_runtime_instance_number()
    {
        runtime* rt = get_runtime_ptr();
        return (NULL == rt) ? 0 : rt->get_instance_number();
    }

    std::string get_config_entry(std::string const& key, std::string const& dflt)
    {
        if (NULL == get_runtime_ptr())
            return "";
        return get_runtime().get_config().get_entry(key, dflt);
    }

    std::string get_config_entry(std::string const& key, std::size_t dflt)
    {
        if (NULL == get_runtime_ptr())
            return "";
        return get_runtime().get_config().get_entry(key, dflt);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Helpers
    naming::id_type find_here(error_code& ec)
    {
        if (NULL == hpx::applier::get_applier_ptr())
        {
            HPX_THROWS_IF(ec, invalid_status, "hpx::find_here",
                "the runtime system is not available at this time");
            return naming::invalid_id;
        }

        return naming::id_type(hpx::applier::get_applier().get_raw_locality(ec),
            naming::id_type::unmanaged);
    }

    std::vector<naming::id_type>
    find_all_localities(components::component_type type, error_code& ec)
    {
        std::vector<naming::id_type> locality_ids;
        if (NULL == hpx::applier::get_applier_ptr())
        {
            HPX_THROWS_IF(ec, invalid_status, "hpx::find_all_localities",
                "the runtime system is not available at this time");
            return locality_ids;
        }

        hpx::applier::get_applier().get_localities(locality_ids, type, ec);
        return locality_ids;
    }

    std::vector<naming::id_type> find_all_localities(error_code& ec)
    {
        std::vector<naming::id_type> locality_ids;
        if (NULL == hpx::applier::get_applier_ptr())
        {
            HPX_THROWS_IF(ec, invalid_status, "hpx::find_all_localities",
                "the runtime system is not available at this time");
            return locality_ids;
        }

        hpx::applier::get_applier().get_localities(locality_ids, ec);
        return locality_ids;
    }

    std::vector<naming::id_type>
    find_remote_localities(components::component_type type)
    {
        std::vector<naming::id_type> locality_ids;
        if (NULL != hpx::applier::get_applier_ptr())
            hpx::applier::get_applier().get_remote_localities(locality_ids, type);
        return locality_ids;
    }

    std::vector<naming::id_type> find_remote_localities()
    {
        std::vector<naming::id_type> locality_ids;
        if (NULL != hpx::applier::get_applier_ptr())
            hpx::applier::get_applier().get_remote_localities(locality_ids);
        return locality_ids;
    }

    // find a locality supporting the given component
    naming::id_type find_locality(components::component_type type, error_code& ec)
    {
        if (NULL == hpx::applier::get_applier_ptr())
        {
            HPX_THROWS_IF(ec, invalid_status, "hpx::find_locality",
                "the runtime system is not available at this time");
            return naming::invalid_id;
        }

        std::vector<naming::id_type> locality_ids;
        hpx::applier::get_applier().get_localities(locality_ids, type, ec);

        if (ec || locality_ids.empty())
            return naming::invalid_id;

        // chose first locality to host the object
        return locality_ids.front();
    }

    /// \brief Return the number of localities which are currently registered
    ///        for the running application.
    boost::uint32_t get_num_localities(error_code& ec)
    {
        if (NULL == hpx::get_runtime_ptr())
            return 0;

        return get_runtime().get_agas_client().get_num_localities(ec);
    }

    boost::uint32_t get_num_localities(components::component_type type,
        error_code& ec)
    {
        if (NULL == hpx::get_runtime_ptr())
            return 0;

        return get_runtime().get_agas_client().get_num_localities(type, ec);
    }

    lcos::future<boost::uint32_t> get_num_localities_async()
    {
        if (NULL == hpx::get_runtime_ptr())
            return lcos::make_future<boost::uint32_t>(0);

        return get_runtime().get_agas_client().get_num_localities_async();
    }

    lcos::future<boost::uint32_t> get_num_localities_async(
        components::component_type type)
    {
        if (NULL == hpx::get_runtime_ptr())
            return lcos::make_future<boost::uint32_t>(0);

        return get_runtime().get_agas_client().get_num_localities_async(type);
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        naming::gid_type get_next_id()
        {
            if (NULL == get_runtime_ptr())
                return naming::invalid_gid;

            return get_runtime().get_next_id();
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    std::size_t get_os_thread_count()
    {
        runtime* rt = get_runtime_ptr();
        if (NULL == rt)
            return 0;
        return rt->get_config().get_os_thread_count();
    }

    std::size_t get_worker_thread_num()
    {
        runtime* rt = get_runtime_ptr();
        if (NULL == rt)
            return std::size_t(-1);
        return rt->get_thread_manager().get_worker_thread_num();
    }

    bool is_scheduler_numa_sensitive()
    {
        runtime* rt = get_runtime_ptr();
        if (NULL == rt)
            return false;

        bool numa_sensitive = false;
        if (std::size_t(-1) != rt->get_thread_manager().get_worker_thread_num(&numa_sensitive))
            return numa_sensitive;
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool keep_factory_alive(components::component_type type)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt)
            return rt->keep_factory_alive(type);
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    components::server::runtime_support* get_runtime_support_ptr()
    {
        return reinterpret_cast<components::server::runtime_support*>(
            get_runtime().get_runtime_support_lva());
    }

    ///////////////////////////////////////////////////////////////////////////
    bool is_running()
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt)
            return rt->get_state() == runtime::state_running;
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace util
{
    std::string expand(std::string const& in)
    {
        return get_runtime().get_config().expand(in);
    }

    void expand(std::string& in)
    {
        get_runtime().get_config().expand(in, std::string::size_type(-1));
    }
}}

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace naming
{
    // shortcut for get_runtime().get_agas_client()
    resolver_client& get_agas_client()
    {
        return get_runtime().get_agas_client();
    }
}}

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace threads
{
    // shortcut for get_applier().get_thread_manager()
    threadmanager_base& get_thread_manager()
    {
        return get_runtime().get_thread_manager();
    }

    // shortcut for runtime_configuration::get_default_stack_size
    std::ptrdiff_t get_default_stack_size()
    {
        return get_runtime().get_config().get_default_stack_size();
    }

    // shortcut for runtime_configuration::get_stack_size
    std::ptrdiff_t get_stack_size(threads::thread_stacksize stacksize)
    {
        return get_runtime().get_config().get_stack_size(stacksize);
    }
}}

///////////////////////////////////////////////////////////////////////////////
namespace hpx
{
    boost::uint32_t get_locality_id(error_code& ec)
    {
        return agas::get_locality_id(ec);
    }

    std::string const* get_thread_name()
    {
        return runtime::get_thread_name();
    }

    boost::uint64_t get_system_uptime()
    {
        return runtime::get_system_uptime();
    }

    util::runtime_configuration const& get_config()
    {
        return get_runtime().get_config();
    }

    hpx::util::io_service_pool* get_thread_pool(char const* name)
    {
        return get_runtime().get_thread_pool(name);
    }

    ///////////////////////////////////////////////////////////////////////////
    void start_active_counters(error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt) {
            rt->start_active_counters(ec);
        }
        else {
            HPX_THROWS_IF(ec, invalid_status, "start_active_counters",
                "the runtime system is not available at this time");
        }
    }

    void stop_active_counters(error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt) {
            rt->stop_active_counters(ec);
        }
        else {
            HPX_THROWS_IF(ec, invalid_status, "stop_active_counters",
                "the runtime system is not available at this time");
        }
    }

    void reset_active_counters(error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt) {
            rt->reset_active_counters(ec);
        }
        else {
            HPX_THROWS_IF(ec, invalid_status, "reset_active_counters",
                "the runtime system is not available at this time");
        }
    }

    void evaluate_active_counters(bool reset, char const* description, error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt) {
            rt->evaluate_active_counters(reset, description, ec);
        }
        else {
            HPX_THROWS_IF(ec, invalid_status, "evaluate_active_counters",
                "the runtime system is not available at this time");
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Create an instance of a message handler plugin
    parcelset::policies::message_handler* create_message_handler(
        char const* message_handler_type, char const* action,
        parcelset::parcelport* pp, std::size_t num_messages,
        std::size_t interval, error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt) {
            return rt->create_message_handler(message_handler_type, action,
                pp, num_messages, interval, ec);
        }

        HPX_THROWS_IF(ec, invalid_status, "create_message_handler",
            "the runtime system is not available at this time");
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Create an instance of a binary filter plugin
    util::binary_filter* create_binary_filter(char const* binary_filter_type,
        bool compress, error_code& ec)
    {
        runtime* rt = get_runtime_ptr();
        if (NULL != rt)
            return rt->create_binary_filter(binary_filter_type, compress, ec);

        HPX_THROWS_IF(ec, invalid_status, "create_binary_filter",
            "the runtime system is not available at this time");
        return 0;
    }
}

