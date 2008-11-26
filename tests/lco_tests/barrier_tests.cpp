//  Copyright (c) 2007-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/lcos/barrier.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/detail/atomic_count.hpp>

using namespace hpx;

///////////////////////////////////////////////////////////////////////////////
threads::thread_state barrier_test(applier::applier& appl, lcos::barrier& b, 
    boost::detail::atomic_count& c)
{
    ++c;
    b.wait();

    // all of the 4 threads need to have incremented the counter
    BOOST_TEST(4 == c);

    return threads::terminated;
}

///////////////////////////////////////////////////////////////////////////////
threads::thread_state hpx_main(applier::applier& appl, 
    lcos::barrier& b, boost::detail::atomic_count& counter)
{
    // create the 4 threads which will have to wait on the barrier
    threads::threadmanager& tm = appl.get_thread_manager();
    for (std::size_t i = 0; i < 4; ++i) {
        register_work(appl, boost::bind(&barrier_test, boost::ref(appl),
            boost::ref(b), boost::ref(counter)));
    }

    b.wait();     // wait for all threads to enter the barrier

    // all of the 4 threads need to have incremented the counter
    BOOST_TEST(4 == counter);

    // initiate shutdown of the runtime system
    components::stubs::runtime_support::shutdown_all(appl);

    return threads::terminated;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    try {
        // Check command line arguments.
        std::string host;
        boost::uint16_t ps_port, agas_port;

        // Check command line arguments.
        if (argc != 4) {
            std::cerr << "Usage: barrier_tests hpx_addr hpx_port agas_port" 
                << std::endl;
            return -1;
        }
        else {
            host = argv[1];
            ps_port = boost::lexical_cast<boost::uint16_t>(argv[2]);
            agas_port  = boost::lexical_cast<boost::uint16_t>(argv[3]);
        }

        // initialize the AGAS service
        hpx::util::io_service_pool agas_pool; 
        hpx::naming::resolver_server agas(agas_pool, 
            hpx::naming::locality(host, agas_port));

        // start the HPX runtime using different numbers of threads
        for (int i = 1; i <= 8; ++i) {
            hpx::runtime rt(host, ps_port, host, agas_port);

            lcos::barrier b(5);       // create a barrier waiting on 5 threads
            boost::detail::atomic_count counter(0);

            rt.run(boost::bind(hpx_main, _1, boost::ref(b), 
                boost::ref(counter)), i);
        }
    }
    catch (std::exception& e) {
        BOOST_TEST(false);
        std::cerr << "std::exception caught: " << e.what() << "\n";
    }
    catch (...) {
        BOOST_TEST(false);
        std::cerr << "unexpected exception caught\n";
    }
    return boost::report_errors();
}
