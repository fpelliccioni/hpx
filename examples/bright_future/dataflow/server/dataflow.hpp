
//  Copyright (c) 2011 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXAMPLES_BRIGHT_FUTURE_DATAFLOW_SERVER_DATAFLOW_HPP
#define EXAMPLES_BRIGHT_FUTURE_DATAFLOW_SERVER_DATAFLOW_HPP

#include <hpx/config.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/naming/name.hpp>
#include <hpx/lcos/base_lco.hpp>
#include <hpx/runtime/actions/component_action.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>

#include <examples/bright_future/dataflow/server/detail/dataflow_impl.hpp>
#include <examples/bright_future/dataflow/server/detail/component_wrapper.hpp>

namespace hpx { namespace lcos {
    namespace server
    {
        /// The dataflow server side representation
        struct dataflow
            : components::managed_component_base<dataflow>
        {
            dataflow()
                : component_ptr(0)
            {}

            ~dataflow()
            {
                LLCO_(info)
                    << "~server::dataflow::dataflow()";
                delete component_ptr;
            }
            
            /// init initializes the dataflow, it creates a dataflow_impl object
            /// that holds old type information and does the remaining processing
            /// of managing the dataflow.
            /// init is a variadic function. The first template parameter denotes
            /// the Action that needs to get spawned once all arguments are
            /// computed
            template <typename Action>
            void init(naming::id_type const & target)
            {
                typedef detail::dataflow_impl<Action> wrapped_type;
                typedef
                    detail::component_wrapper<wrapped_type> 
                    component_type;
                
                LLCO_(info)
                    << "server::dataflow::init() " << get_gid();

                component_type * w = new component_type(target);
                (*w)->init();
                component_ptr = w;
            }
            
            /// init_action is the action that can be used to call the variadic
            /// function from a client
            template <
                typename Action
              , BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
                    HPX_ACTION_ARGUMENT_LIMIT
                  , typename A
                  , void
                )
              , typename Enable = void
            >
            struct init_action;

            template <typename Action>
            struct init_action<Action>
            {
                typedef
                    ::hpx::actions::direct_action1<
                        dataflow
                      , 0
                      , naming::id_type const &
                      , &dataflow::init<Action>
                    >
                    type;
            };

            // Vertical preprocessor repetition to define the remaining
            // init functions and actions
            // TODO: get rid of the call to impl_ptr->init
#define HPX_LCOS_DATAFLOW_M0(Z, N, D)                                           \
            template <typename Action, BOOST_PP_ENUM_PARAMS(N, typename A)>     \
            void init(                                                    \
                naming::id_type const & target                                  \
              , BOOST_PP_ENUM_BINARY_PARAMS(N, A, const & a)                    \
            )                                                                   \
            {                                                                   \
                typedef                                                         \
                    detail::dataflow_impl<                                      \
                        Action                                                  \
                      , BOOST_PP_ENUM_PARAMS(N, A)                              \
                    >                                                           \
                    wrapped_type;                                               \
                                                                                \
                typedef                                                         \
                    detail::component_wrapper<                                  \
                        wrapped_type                                            \
                    >                                                           \
                    component_type;                                             \
                component_type * w = new component_type(target);                \
                (*w)->init(BOOST_PP_ENUM_PARAMS(N, a));                         \
                component_ptr = w;                                              \
            }                                                                   \
                                                                                \
            template <typename Action, BOOST_PP_ENUM_PARAMS(N, typename A)>     \
            struct init_action<Action, BOOST_PP_ENUM_PARAMS(N, A)>              \
            {                                                                   \
                typedef                                                         \
                    BOOST_PP_CAT(hpx::actions::direct_action, BOOST_PP_INC(N))< \
                        dataflow                                                \
                      , 0                                                       \
                      , naming::id_type const &                                 \
                      , BOOST_PP_ENUM_BINARY_PARAMS(                            \
                            N                                                   \
                          , A                                                   \
                          , const & BOOST_PP_INTERCEPT                          \
                        )                                                       \
                      , &dataflow::init<                                        \
                            Action                                              \
                          , BOOST_PP_ENUM_PARAMS(N, A)                          \
                        >                                                       \
                    >                                                           \
                    type;                                                       \
            };                                                                  \
    /**/
        BOOST_PP_REPEAT_FROM_TO(
            1
          , HPX_ACTION_ARGUMENT_LIMIT
          , HPX_LCOS_DATAFLOW_M0
          , _
        )
#undef HPX_LCOS_DATAFLOW_M0
            
            /// the connect function is used to connect the current dataflow
            /// to the specified target lco
            void connect(naming::id_type const & target)
            {
                (*component_ptr)->connect_nonvirt(target);
            }

            typedef 
                ::hpx::actions::direct_action1<
                    dataflow
                  , 0
                  , naming::id_type const &
                  , &dataflow::connect
                >
                connect_action;

        private:
            detail::component_wrapper_base * component_ptr;
        };
    }
}}


#endif