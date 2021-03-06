# Copyright (c) 2012      Andrew Kemp
# Copyright (c) 2007-2012 Hartmut Kaiser
# Copyright (c) 2011      Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

find_package(HPX_OpenCL)

if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
  find_package(HPX_OCLM)
endif()

if(OCLM_FOUND)
   include_directories(${OCLM_INCLUDE_DIR})
   set(hpx_RUNTIME_LIBRARIES ${hpx_RUNTIME_LIBRARIES} ${OCLM_LIBRARY})

  set(example_programs
      oclm_test)

  foreach(example_program ${example_programs})
    set(sources
        ${example_program}.cpp)

    source_group("Source Files" FILES ${sources})

    # add example executable
    add_hpx_executable(${example_program}
                       SOURCES ${sources}
                       ${${example_program}_FLAGS}
                       DEPENDENCIES ${OpenCL_LIBRARY} ${OCLM_LIBRARY}
                       FOLDER "Examples/OCLM Integration/${example_program}")

    # add a custom target for this example
    add_hpx_pseudo_target(examples.oclm_integration.${example_program})

    # make pseudo-targets depend on master pseudo-target
    add_hpx_pseudo_dependencies(examples.oclm_integration
                                examples.oclm_integration.${example_program})

    # add dependencies to pseudo-target
    add_hpx_pseudo_dependencies(examples.oclm_integration.${example_program}
                                ${example_program}_exe)
  endforeach()

endif()
