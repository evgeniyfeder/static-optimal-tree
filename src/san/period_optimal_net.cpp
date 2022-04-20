//
// Created by evgeniyfeder on 19/11/2021.
//

#include "san/period_optimal_net.h"

period_optimal_net_t::period_optimal_net_t(adjacency_matrix_t m)
   : net_t(make_optimal_net(m).opt_root), m_(m), counter(0)
{
}

int period_optimal_net_t::process_request(int from, int to)
{
   int result = net_nodes_[from]->route(to);

   if (counter >= m_.size() * m_.size())
   {
      auto new_network = make_optimal_net(m_);

      root_ = std::make_shared<net_node_t>(*(new_network.opt_root));
      net_nodes_.clear();
      fill_net_nodes(root_);

      counter = 0;
   }

   counter++;
   m_.add_request(from, to);
   return result;
}


