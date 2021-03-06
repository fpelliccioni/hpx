// Copyright (c) 2007-2013 Hartmut Kaiser
// Copyright (c) 2012-2013 Thomas Heller
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file has been automatically generated using the Boost.Wave tool.
// Do not edit manually.


namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(1);
        lazy_values.push_back(f0);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(2);
        lazy_values.push_back(f0); lazy_values.push_back(f1);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(3);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(4);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(5);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(6);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(7);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(8);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(9);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(10);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(11);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9); lazy_values.push_back(f10);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9 , f10);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(12);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9); lazy_values.push_back(f10); lazy_values.push_back(f11);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9 , f10 , f11);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(13);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9); lazy_values.push_back(f10); lazy_values.push_back(f11); lazy_values.push_back(f12);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9 , f10 , f11 , f12);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12 , lcos::future<T> f13)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(14);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9); lazy_values.push_back(f10); lazy_values.push_back(f11); lazy_values.push_back(f12); lazy_values.push_back(f13);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12 , lcos::future<T> f13,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9 , f10 , f11 , f12 , f13);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
namespace hpx
{
    
    template <typename T>
    lcos::future<std::vector<lcos::future<T> > >
    when_all (lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12 , lcos::future<T> f13 , lcos::future<T> f14)
    {
        typedef std::vector<lcos::future<T> > return_type;
        return_type lazy_values;
        lazy_values.reserve(15);
        lazy_values.push_back(f0); lazy_values.push_back(f1); lazy_values.push_back(f2); lazy_values.push_back(f3); lazy_values.push_back(f4); lazy_values.push_back(f5); lazy_values.push_back(f6); lazy_values.push_back(f7); lazy_values.push_back(f8); lazy_values.push_back(f9); lazy_values.push_back(f10); lazy_values.push_back(f11); lazy_values.push_back(f12); lazy_values.push_back(f13); lazy_values.push_back(f14);
        lcos::local::futures_factory<return_type()> p(
            detail::when_all<T>(boost::move(lazy_values)));
        p.apply();
        return p.get_future();
    }
    template <typename T>
    std::vector<lcos::future<T> >
    wait_all(lcos::future<T> f0 , lcos::future<T> f1 , lcos::future<T> f2 , lcos::future<T> f3 , lcos::future<T> f4 , lcos::future<T> f5 , lcos::future<T> f6 , lcos::future<T> f7 , lcos::future<T> f8 , lcos::future<T> f9 , lcos::future<T> f10 , lcos::future<T> f11 , lcos::future<T> f12 , lcos::future<T> f13 , lcos::future<T> f14,
        error_code& ec = throws)
    {
        typedef std::vector<lcos::future<T> > result_type;
        lcos::future<result_type> f = when_all(
            f0 , f1 , f2 , f3 , f4 , f5 , f6 , f7 , f8 , f9 , f10 , f11 , f12 , f13 , f14);
        if (!f.valid()) {
            { if (&ec == &hpx::throws) { HPX_THROW_EXCEPTION( uninitialized_value, "lcos::wait_all", "lcos::when_all didn't return a valid future"); } else { ec = make_error_code(static_cast<hpx::error>( uninitialized_value), "lcos::when_all didn't return a valid future", "lcos::wait_all", "D:/Devel\\hpx\\hpx\\lcos\\wait_all.hpp", 435, (ec.category() == hpx::get_lightweight_hpx_category()) ? hpx::lightweight : hpx::plain); } };
            return result_type();
        }
        return f.get(ec);
    }
}
