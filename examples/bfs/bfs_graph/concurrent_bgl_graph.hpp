//  Copyright (c) 2007-2012 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_EXAMPLE_BFS_CONCURRENT_BGL_GRAPH_JAN_02_2012_0529PM)
#define HPX_EXAMPLE_BFS_CONCURRENT_BGL_GRAPH_JAN_02_2012_0529PM

#include <hpx/include/client.hpp>
#include <boost/assert.hpp>

#include "stubs/concurrent_bgl_graph.hpp"

namespace bfs
{
    ///////////////////////////////////////////////////////////////////////////
    /// The client side representation of a \a bfs::server::graph components.
    class concurrent_bgl_graph
      : public hpx::components::client_base<
            concurrent_bgl_graph, stubs::concurrent_bgl_graph>
    {
        typedef hpx::components::client_base<
            concurrent_bgl_graph, stubs::concurrent_bgl_graph>
        base_type;

    public:
        /// Default construct an empty client side representation (not
        /// connected to any existing component).
        concurrent_bgl_graph()
        {}

        /// Create a client side representation for the existing
        /// \a bfs::server::graph instance with the given GID.
        concurrent_bgl_graph(hpx::naming::id_type const& gid)
          : base_type(gid)
        {}

        ///////////////////////////////////////////////////////////////////////
        // Exposed functionality of this component.

        // initialize the graph
        hpx::lcos::future<void> init_async(
            std::size_t idx, std::size_t grainsize,
            std::vector<std::pair<std::size_t, std::size_t> > const& edgelist)
        {
            return this->base_type::init_async(get_gid(), idx, grainsize, edgelist);
        }
        void init(std::size_t idx, std::size_t grainsize,
            std::vector<std::pair<std::size_t, std::size_t> > const& edgelist)
        {
            this->base_type::init_async(get_gid(), idx, grainsize, edgelist);
        }

        /// Perform a BFS on the graph.
        hpx::lcos::future<double>
        bfs_async(std::size_t root)
        {
            return this->base_type::bfs_async(get_gid(), root);
        }
        double bfs(std::size_t root)
        {
            return this->base_type::bfs(get_gid(), root);
        }

        /// validate the BFS on the graph.
        hpx::lcos::future<std::vector<std::size_t> >
        get_parents_async()
        {
            return this->base_type::get_parents_async(get_gid());
        }
        std::vector<std::size_t> get_parents()
        {
            return this->base_type::get_parents(get_gid());
        }

        /// Reset for the next BFS
        hpx::lcos::future<void>
        reset_async()
        {
            return this->base_type::reset_async(get_gid());
        }
        void reset()
        {
            this->base_type::reset(get_gid());
        }
    };
}

#endif

