# Copyright (c) 2007-2012 Hartmut Kaiser
# Copyright (c) 2011 Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    dataflow_test
    trivial_future)

set(dataflow_test_FLAGS
  DEPENDENCIES iostreams_component dataflow_component)

foreach(test ${tests})
  set(sources
      ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${test}
                     SOURCES ${sources}
                     ${${test}_FLAGS}
                     FOLDER "Tests/Unit/LCOs")

  # add a custom target for this example
  add_hpx_pseudo_target(tests.unit.lcos.${test})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.unit.lcos
                              tests.unit.lcos.${test})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.unit.lcos.${test}
                              ${test}_exe)
endforeach()

set_target_properties(dataflow_test_exe
    PROPERTIES COMPILE_FLAGS
        "-DHPX_ACTION_ARGUMENT_LIMIT=10 -DHPX_COMPONENT_CREATE_ARG_MAX=10")
