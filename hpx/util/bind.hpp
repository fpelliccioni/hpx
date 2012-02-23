//  Copyright (c) 2011 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

#ifndef HPX_UTIL_BIND_HPP
#define HPX_UTIL_BIND_HPP

#include <hpx/config.hpp>
#include <hpx/util/tuple.hpp>

#include <boost/get_pointer.hpp>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>

#include <boost/move/move.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#include <boost/type_traits/remove_reference.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace util {
    namespace detail
    {
#if defined(BOOST_NO_RVALUE_REFERENCES)
        template <typename T>
        struct remove_reference
        {
            typedef typename boost::remove_reference<T>::type type;
        };

        template <typename T>
        struct remove_reference<boost::rv<T> >
        {
            typedef T type;
        };

        template <typename T>
        struct env_value_type;

        template <typename T>
        struct env_value_type<T &>
        {
            typedef typename remove_reference<T>::type & type;
        };

        template <typename T>
        struct env_value_type<T const &>
        {
            typedef typename remove_reference<T>::type const & type;
        };

#else
        using std::remove_reference;

        template <typename T, bool IsRvalue = std::is_rvalue_reference<T>::value>
        struct env_value_type
        {
            typedef typename remove_reference<T>::type type;
        };

        template <typename T>
        struct env_value_type<T &, false>
        {
            typedef T & type;
        };

        template <typename T>
        struct env_value_type<T const &, false>
        {
            typedef T const & type;
        };
#endif

        template <typename Env, typename T>
        T & eval(Env const &, T & t)
        {
            return t;
        }

        template <typename Env, typename T>
        T const & eval(Env const &, T const & t)
        {
            return t;
        }

        /*
        template <typename T>
        T * get_pointer(T &t)
        {
            return &t;
        }

        template <typename T>
        T const * get_pointer(T const &t)
        {
            return &t;
        }
        */
    }

    namespace placeholders
    {
        template <int N>
        struct arg {};

#define HPX_UTIL_BIND_PLACEHOLDERS(Z, N, D)                                     \
        typedef arg<N> BOOST_PP_CAT(BOOST_PP_CAT(arg_, BOOST_PP_INC(N)), _type);\
        BOOST_PP_CAT(BOOST_PP_CAT(arg_, BOOST_PP_INC(N)), _type) const          \
            BOOST_PP_CAT(arg_, BOOST_PP_INC(N)) = {};                           \
        BOOST_PP_CAT(BOOST_PP_CAT(arg_, BOOST_PP_INC(N)), _type) const          \
            BOOST_PP_CAT(_, BOOST_PP_INC(N)) = {};                              \
/**/
        BOOST_PP_REPEAT(HPX_FUNCTION_LIMIT, HPX_UTIL_BIND_PLACEHOLDERS, _)
#undef HPX_UTIL_BIND_PLACEHOLDERS
    }

    namespace detail
    {
        template <typename Env, int N>
        typename boost::fusion::result_of::at_c<Env, N>::type
        eval(Env const & env, placeholders::arg<N> const&)
        {
            return boost::fusion::at_c<N>(env);
        }
    }

#define HPX_UTIL_BIND_FWD_REF_PARAMS(Z, N, D)                                   \
    BOOST_FWD_REF(BOOST_PP_CAT(A, N)) BOOST_PP_CAT(a, N)                        \
/**/

#define HPX_UTIL_BIND_FWD_PARAMS(Z, N, D)                                       \
    boost::forward<BOOST_PP_CAT(D, N)>(BOOST_PP_CAT(a, N))                      \
/**/

#define HPX_UTIL_BIND_EVAL(Z, N, D)                                             \
    ::hpx::util::detail::eval(env, BOOST_PP_CAT(arg, N))                        \
/**/

#define HPX_UTIL_BIND_REMOVE_REFERENCE(Z, N, D)                                 \
    typename boost::remove_const<                                               \
        typename detail::remove_reference<BOOST_PP_CAT(D, N)>::type>::type      \
/**/

#define HPX_UTIL_BIND_REFERENCE(Z, N, D)                                        \
    typename detail::env_value_type<BOOST_FWD_REF(BOOST_PP_CAT(D, N))>::type    \
/**/

#define HPX_UTIL_BIND_FUNCTOR_OPERATOR(Z, N, D)                                 \
    template <BOOST_PP_ENUM_PARAMS(N, typename A)>                              \
    result_type operator()(BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)) const  \
    {                                                                           \
        typedef                                                                 \
            BOOST_PP_CAT(hpx::util::tuple, N)<                                  \
                BOOST_PP_ENUM(N, HPX_UTIL_BIND_REFERENCE, A)                    \
            >                                                                   \
            env_type;                                                           \
        env_type env = {BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A)};         \
        return eval(env, f) D;                                                  \
    }                                                                           \
/**/

    ///////////////////////////////////////////////////////////////////////////
    // free functions
    namespace detail
    {
        template <typename R>
        struct bound_function0
        {
            typedef R result_type;
            R(*f)();

            bound_function0(R(*f)())
                : f(f)
            {}

            R operator()() const
            {
                return f();
            }

            BOOST_PP_REPEAT_FROM_TO(1, HPX_FUNCTION_LIMIT,
                HPX_UTIL_BIND_FUNCTOR_OPERATOR, ())
        };

        template <typename Env, typename R>
        R
        eval(Env const & env, util::detail::bound_function0<R> const & f)
        {
            return f();
        }

        template <typename Env, typename R>
        R
        eval(Env const & env, util::detail::bound_function0<R> & f)
        {
            return f();
        }
    }

    template <typename R>
    detail::bound_function0<R>
    bind(R(*f)())
    {
        return detail::bound_function0<R>(f);
    }

    ///////////////////////////////////////////////////////////////////////////
    // functor
    namespace detail
    {
        template <typename F>
        struct bound_functor0
        {
            F f;

            typedef typename F::result_type result_type;

            bound_functor0(bound_functor0 const & other)
                : f(other.f)
            {}

            bound_functor0(BOOST_RV_REF(bound_functor0) other)
                : f(boost::move(other.f))
            {}

            bound_functor0 & operator=(BOOST_COPY_ASSIGN_REF(bound_functor0) other)
            {
                f = other.f;
                return *this;
            }

            bound_functor0 & operator=(BOOST_RV_REF(bound_functor0) other)
            {
                f = boost::move(other.f);
                return *this;
            }

            bound_functor0(BOOST_FWD_REF(F) f)
                : f(f)
            {}

            result_type operator()() const
            {
                return f();
            }

            BOOST_PP_REPEAT_FROM_TO(1, HPX_FUNCTION_LIMIT,
                HPX_UTIL_BIND_FUNCTOR_OPERATOR, ())

        private:
            BOOST_COPYABLE_AND_MOVABLE(bound_functor0);
        };

        template <typename Env, typename F>
        typename F::result_type
        eval(Env const & env, util::detail::bound_functor0<F> const & f)
        {
            return f();
        }
    }

    template <typename F>
    detail::bound_functor0<F>
    bind(BOOST_FWD_REF(F) f)
    {
        return detail::bound_functor0<F>(boost::forward<F>(f));
    }

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (                                                                           \
        3                                                                       \
      , (                                                                       \
            1                                                                   \
          , HPX_FUNCTION_LIMIT                                                  \
          , <hpx/util/bind.hpp>                                                 \
        )                                                                       \
    )                                                                           \
/**/
#include BOOST_PP_ITERATE()

#undef HPX_UTIL_BIND_FWD_REF_PARAMS
}}

#endif

#else  // !BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()
#define NN N

#define HPX_UTIL_BIND_INIT_MEMBER(Z, N, D)                                      \
    BOOST_PP_CAT(arg, N)(boost::forward<BOOST_PP_CAT(A, N)>(BOOST_PP_CAT(a, N)))\
/**/
#define HPX_UTIL_BIND_MEMBER(Z, N, D)                                           \
    BOOST_PP_CAT(Arg, N) BOOST_PP_CAT(arg, N);                                  \
/**/

#define HPX_UTIL_BIND_INIT_COPY_MEMBER(Z, N, D)                                 \
    BOOST_PP_CAT(arg, N)(other.BOOST_PP_CAT(arg, N))                            \
/**/

#define HPX_UTIL_BIND_INIT_MOVE_MEMBER(Z, N, D)                                 \
    BOOST_PP_CAT(arg, N)(boost::move(other.BOOST_PP_CAT(arg, N)))               \
/**/

#define HPX_UTIL_BIND_ASSIGN_MEMBER(Z, N, D)                                    \
    BOOST_PP_CAT(arg, N) = other.BOOST_PP_CAT(arg, N);                          \
/**/

#define HPX_UTIL_BIND_MOVE_MEMBER(Z, N, D)                                      \
    BOOST_PP_CAT(arg, N) = boost::move(other.BOOST_PP_CAT(arg, N));             \
/**/

    ///////////////////////////////////////////////////////////////////////////
    // free functions
    namespace detail
    {
        template <
            typename R
          , BOOST_PP_ENUM_PARAMS(N, typename T)
          , BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        struct BOOST_PP_CAT(bound_function, N)
        {
            typedef R result_type;
            typedef R(*function_pointer_type)(BOOST_PP_ENUM_PARAMS(N, T));
            function_pointer_type f;

            BOOST_PP_CAT(bound_function, N)(
                    BOOST_PP_CAT(bound_function, N) const & other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_COPY_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_function, N)(
                    BOOST_RV_REF(BOOST_PP_CAT(bound_function, N)) other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MOVE_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_function, N) & operator=(
                BOOST_COPY_ASSIGN_REF(BOOST_PP_CAT(bound_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_ASSIGN_MEMBER, _)
                return *this;
            }

            BOOST_PP_CAT(bound_function, N) & operator=(
                BOOST_RV_REF(BOOST_PP_CAT(bound_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MOVE_MEMBER, _)
                return *this;
            }

            template <BOOST_PP_ENUM_PARAMS(N, typename A)>
            BOOST_PP_CAT(bound_function, N)(
                function_pointer_type f
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
            )
                : f(f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MEMBER, _)
            {}

            R operator()() const
            {
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return f(BOOST_PP_ENUM(N, HPX_UTIL_BIND_EVAL, _));
            }

            BOOST_PP_REPEAT_FROM_TO(1, HPX_FUNCTION_LIMIT,
                HPX_UTIL_BIND_FUNCTOR_OPERATOR,
                (BOOST_PP_ENUM(N, HPX_UTIL_BIND_EVAL, _)))

            BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MEMBER, _)

        private:
            BOOST_COPYABLE_AND_MOVABLE(BOOST_PP_CAT(bound_function, N));
        };

        template <
            typename Env
          , typename R
          , BOOST_PP_ENUM_PARAMS(N, typename T)
          , BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        R
        eval(
            Env const & env
          , BOOST_PP_CAT(hpx::util::detail::bound_function, N)<
                R
              , BOOST_PP_ENUM_PARAMS(N, T)
              , BOOST_PP_ENUM_PARAMS(N, Arg)
            > const & f
        )
        {
            return
                boost::fusion::fused<
                    BOOST_PP_CAT(hpx::util::detail::bound_function, N)<
                        R
                      , BOOST_PP_ENUM_PARAMS(N, T)
                      , BOOST_PP_ENUM_PARAMS(N, Arg)
                    >
                >(f)(
                    env
                 );
        }
    }

    template <
        typename R
      , BOOST_PP_ENUM_PARAMS(N, typename T)
      , BOOST_PP_ENUM_PARAMS(N, typename A)
    >
    BOOST_PP_CAT(detail::bound_function, N)<
        R
      , BOOST_PP_ENUM_PARAMS(N, T)
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
    >
    bind(
        R(*f)(BOOST_PP_ENUM_PARAMS(N, T))
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
    )
    {
        return
            BOOST_PP_CAT(detail::bound_function, N)<
                R
              , BOOST_PP_ENUM_PARAMS(N, T)
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
            >
            (f, BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A));
    }

    ///////////////////////////////////////////////////////////////////////////
    // member function pointers
    namespace detail
    {
        template <
            typename R
          , typename C
          , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), typename T)
          BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        struct BOOST_PP_CAT(bound_member_function, N)
        {
            typedef R result_type;
            typedef R(C::*function_pointer_type)(
                BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T));
            function_pointer_type f;

            template <BOOST_PP_ENUM_PARAMS(N, typename A)>
            BOOST_PP_CAT(bound_member_function, N)(
                function_pointer_type f
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
            )
                : f(f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N)(
                    BOOST_PP_CAT(bound_member_function, N) const & other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_COPY_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N)(BOOST_RV_REF(
                    BOOST_PP_CAT(bound_member_function, N)) other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MOVE_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N) & operator=(
                BOOST_COPY_ASSIGN_REF(BOOST_PP_CAT(bound_member_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_ASSIGN_MEMBER, _)
                return *this;
            }

            BOOST_PP_CAT(bound_member_function, N) & operator=(
                BOOST_RV_REF(BOOST_PP_CAT(bound_member_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MOVE_MEMBER, _)
                return *this;
            }

            R operator()() const
            {
                using boost::get_pointer;
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return
                    (get_pointer(detail::eval(env, arg0))->*f)
                        (BOOST_PP_ENUM_SHIFTED(N, HPX_UTIL_BIND_EVAL, _));
            }

            R operator()()
            {
                using boost::get_pointer;
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return
                    (get_pointer(detail::eval(env, arg0))->*f)
                        (BOOST_PP_ENUM_SHIFTED(N, HPX_UTIL_BIND_EVAL, _));
            }

#define HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR(Z, N, D)                          \
    template <BOOST_PP_ENUM_PARAMS(N, typename A)>                              \
    result_type operator()(BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _))   \
    {                                                                           \
        using boost::get_pointer;                                               \
        typedef                                                                 \
            BOOST_PP_CAT(hpx::util::tuple, N)<                                  \
                BOOST_PP_ENUM(N, HPX_UTIL_BIND_REFERENCE, A)                    \
            >                                                                   \
            env_type;                                                           \
        env_type env = {BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A)};         \
        return                                                                  \
            (get_pointer(detail::eval(env, arg0))->*f)                          \
                (BOOST_PP_ENUM_SHIFTED(NN, HPX_UTIL_BIND_EVAL, _));             \
    }                                                                           \
/**/

            BOOST_PP_REPEAT_FROM_TO(
                1
              , HPX_FUNCTION_LIMIT
              , HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR, _
            )
#undef HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR
            BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MEMBER, _)
        };

        template <
            typename R
          , typename C
          , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), typename T)
          BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        struct BOOST_PP_CAT(bound_member_function, N)<
            R
          , C const
          , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
          BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, Arg)
        >
        {
            typedef R result_type;
            typedef R(C::*function_pointer_type)(
                BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)) const;
            function_pointer_type f;

            template <BOOST_PP_ENUM_PARAMS(N, typename A)>
            BOOST_PP_CAT(bound_member_function, N)(
                function_pointer_type f
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
            )
                : f(f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N)(
                    BOOST_PP_CAT(bound_member_function, N) const & other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_COPY_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N)(
                    BOOST_RV_REF(BOOST_PP_CAT(bound_member_function, N)) other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MOVE_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_member_function, N) & operator=(
                BOOST_COPY_ASSIGN_REF(BOOST_PP_CAT(bound_member_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_ASSIGN_MEMBER, _)
                return *this;
            }

            BOOST_PP_CAT(bound_member_function, N) & operator=(
                BOOST_RV_REF(BOOST_PP_CAT(bound_member_function, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MOVE_MEMBER, _)
                return *this;
            }

            R operator()() const
            {
                using boost::get_pointer;
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return
                    (get_pointer(detail::eval(env, arg0))->*f)
                        (BOOST_PP_ENUM_SHIFTED(N, HPX_UTIL_BIND_EVAL, _));
            }

            R operator()()
            {
                using boost::get_pointer;
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return
                    (get_pointer(detail::eval(env, arg0))->*f)
                        (BOOST_PP_ENUM_SHIFTED(N, HPX_UTIL_BIND_EVAL, _));
            }

#define HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR(Z, N, D)                          \
    template <BOOST_PP_ENUM_PARAMS(N, typename A)>                              \
    result_type operator()(BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)) const \
    {                                                                           \
        using boost::get_pointer;                                               \
        typedef                                                                 \
            BOOST_PP_CAT(hpx::util::tuple, N)<                                  \
                BOOST_PP_ENUM(N, HPX_UTIL_BIND_REFERENCE, A)                    \
            >                                                                   \
            env_type;                                                           \
        env_type env = {BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A)};         \
        return                                                                  \
            (get_pointer(detail::eval(env, a0))->*f)                            \
                (BOOST_PP_ENUM_SHIFTED(NN, HPX_UTIL_BIND_EVAL, _));             \
    }                                                                           \
/**/

            BOOST_PP_REPEAT_FROM_TO(
                1
              , HPX_FUNCTION_LIMIT
              , HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR, _
            )
#undef HPX_UTIL_BIND_MEMBER_FUNCTOR_OPERATOR
            BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MEMBER, _)
        };

        template <
            typename Env
          , typename R
          , typename C
          , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), typename T)
          BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
              BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        R
        eval(
            Env const & env
          , BOOST_PP_CAT(hpx::util::detail::bound_member_function, N)<
                R
              , C
              , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
              BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, Arg)
            > const & f
        )
        {
            return
                boost::fusion::fused<
                    BOOST_PP_CAT(hpx::util::detail::bound_member_function, N)<
                        R
                      , C
                      , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
                      BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                          BOOST_PP_ENUM_PARAMS(N, Arg)
                    >
                >(f)(
                    env
                 );
        }
    }

    template <
        typename R
      , typename C
      , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), typename T)
      BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, typename A)
    >
    BOOST_PP_CAT(detail::bound_member_function, N)<
        R
      , C
      , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
      BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
          BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
    >
    bind(
        R(C::*f)(BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T))
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
    )
    {
        return
            BOOST_PP_CAT(detail::bound_member_function, N)<
                R
              , C
              , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
              BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                  BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
            >
            (f, BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A));
    }

    template <
        typename R
      , typename C
      , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), typename T)
      BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) BOOST_PP_ENUM_PARAMS(N, typename A)
    >
    BOOST_PP_CAT(detail::bound_member_function, N)<
        R
      , C const
      , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
      BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
          BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
    >
    bind(
        R(C::*f)(BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)) const
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
    )
    {
        return
            BOOST_PP_CAT(detail::bound_member_function, N)<
                R
              , C const
              , BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(N), T)
              BOOST_PP_COMMA_IF(BOOST_PP_DEC(N))
                  BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
            >
            (f, BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A));
    }

    ///////////////////////////////////////////////////////////////////////////
    // functor
    namespace detail
    {
        template <
            typename F
          , BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        struct BOOST_PP_CAT(bound_functor, N)
        {
            F f;

            typedef typename F::result_type result_type;

            template <BOOST_PP_ENUM_PARAMS(N, typename A)>
            BOOST_PP_CAT(bound_functor, N)(
                BOOST_FWD_REF(F) f
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
            )
                : f(boost::forward<F>(f))
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_functor, N)(
                    BOOST_PP_CAT(bound_functor, N) const & other)
                : f(other.f)
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_COPY_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_functor, N)(
                    BOOST_RV_REF(BOOST_PP_CAT(bound_functor, N)) other)
                : f(boost::move(other.f))
                , BOOST_PP_ENUM(N, HPX_UTIL_BIND_INIT_MOVE_MEMBER, _)
            {}

            BOOST_PP_CAT(bound_functor, N) & operator=(
                BOOST_COPY_ASSIGN_REF(BOOST_PP_CAT(bound_functor, N)) other)
            {
                f = other.f;
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_ASSIGN_MEMBER, _)
                return *this;
            }

            BOOST_PP_CAT(bound_functor, N) & operator=(
                BOOST_RV_REF(BOOST_PP_CAT(bound_functor, N)) other)
            {
                f = boost::move(other.f);
                BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MOVE_MEMBER, _)
                return *this;
            }

            result_type operator()() const
            {
                typedef hpx::util::tuple0<> env_type;
                env_type env;
                return eval(env, f)(BOOST_PP_ENUM(N, HPX_UTIL_BIND_EVAL, _));
            }

            BOOST_PP_REPEAT_FROM_TO(1, HPX_FUNCTION_LIMIT,
                HPX_UTIL_BIND_FUNCTOR_OPERATOR,
                (BOOST_PP_ENUM(N, HPX_UTIL_BIND_EVAL, _)))

            BOOST_PP_REPEAT(N, HPX_UTIL_BIND_MEMBER, _)
        };

        template <
            typename Env
          , typename F
          , BOOST_PP_ENUM_PARAMS(N, typename Arg)
        >
        typename F::result_type
        eval(
            Env const & env
          , BOOST_PP_CAT(util::detail::bound_functor, N)<
                F
              , BOOST_PP_ENUM_PARAMS(N, Arg)
            > const & f
        )
        {
            return
                boost::fusion::fused<
                    BOOST_PP_CAT(hpx::util::detail::bound_functor, N)<
                        F
                      , BOOST_PP_ENUM_PARAMS(N, Arg)
                    >
                >(f)(
                    env
                 );
        }
    }

    template <typename F
      , BOOST_PP_ENUM_PARAMS(N, typename A)
    >
    BOOST_PP_CAT(detail::bound_functor, N)<
        F
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
    >
    bind(
        BOOST_FWD_REF(F) f
      , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_REF_PARAMS, _)
    )
    {
        return
            BOOST_PP_CAT(detail::bound_functor, N)<
                F
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_REMOVE_REFERENCE, A)
            >(
                boost::forward<F>(f)
              , BOOST_PP_ENUM(N, HPX_UTIL_BIND_FWD_PARAMS, A)
            );
    }

#undef NN
#undef N

#endif