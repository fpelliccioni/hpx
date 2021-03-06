////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(HPX_E0A61B00_A571_48EF_8516_ECB48CDDBC00)
#define HPX_E0A61B00_A571_48EF_8516_ECB48CDDBC00

#include <boost/cstdint.hpp>

namespace hpx { namespace util { namespace hardware
{

inline boost::uint64_t timestamp()
{
    boost::uint32_t lo, hi;
    #if defined(HPX_HAVE_RDTSCP)
        __asm__ __volatile__ (
            "rdtscp ;\n"
            : "=a" (lo), "=d" (hi)
            :
            : "rcx");
    #elif defined(HPX_HAVE_RDTSC)
        __asm__ __volatile__ (
            "cpuid ;\n"
            "rdtsc ;\n"
            : "=a" (lo), "=d" (hi)
            :
            : "rbx", "rcx");
    #endif
    return ((static_cast<boost::uint64_t>(hi)) << 32) | lo;
}

}}}

#endif // HPX_E0A61B00_A571_48EF_8516_ECB48CDDBC00

