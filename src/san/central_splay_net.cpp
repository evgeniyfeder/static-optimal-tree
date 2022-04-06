#include "san/central_splay_net.h"

central_splay_net::central_splay_net(std::shared_ptr<node_t> const & tree_root, std::shared_ptr<node_t> const & central_node)
   : net_t(tree_root), central_node(central_node) {}

int central_splay_net::process_request(int from, int to)
{
   int result = net_nodes_[from]->route(to);


   return 0;
}

central_splay_net
{
};