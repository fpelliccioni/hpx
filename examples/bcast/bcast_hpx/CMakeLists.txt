# Copyright (c) 2011 Matt Anderson
# Copyright (c) 2007-2012 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_hpx_component(bcast_hpx
  SOURCE_GLOB "*.cpp"
  HEADER_GLOB "*.hpp"
  DEPENDENCIES iostreams_component 
  FOLDER "Examples/Applications/Bcast"
  AUTOGLOB)

