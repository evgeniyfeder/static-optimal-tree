#include "net.h"

void net_t::fill_net_nodes(std::shared_ptr<net_node_t> const & cur_node)
{
   if (!cur_node)
      return;

   if (net_nodes_.find(cur_node->key) != net_nodes_.end())
      throw std::runtime_error("Duplication of nodes in the net");
   net_nodes_.insert({cur_node->key, cur_node});

   fill_net_nodes(cur_node->left);
   fill_net_nodes(cur_node->right);
}

net_t::net_t(std::shared_ptr<node_t> const & tree_root)
{
   if (!tree_root)
      return;

   root_ = std::make_shared<net_node_t>(*tree_root);
   fill_net_nodes(root_);
}

net_t::net_t(std::shared_ptr<net_node_t> const & tree_root)
{
   if (!tree_root)
      return;

   root_ = tree_root;
   fill_net_nodes(root_);
}

net_node_t::net_node_t(node_t const & node)
      : key(node.key), min_key(key), max_key(key)
{
   if (node.left)
   {
      left = std::make_shared<net_node_t>(*node.left);
      left->parent = this;
      min_key = left->min_key;
   }
   if (node.right)
   {
      right = std::make_shared<net_node_t>(*node.right);
      right->parent = this;
      max_key = right->max_key;
   }
}

int net_node_t::route(int to)
{
   if (to == key)
      return 0;
   if (to < min_key || to > max_key)
      return parent->route(to) + 1;
   if (to < key)
      return left->route(to) + 1;
   if (to > key)
      return right->route(to) + 1;

   throw std::runtime_error("BAD WAY: can't reach this place");
}

int net_node_t::subtree_root(int to)
{
   if (key == to)
      return key;

   if (to < min_key || to > max_key)
      return parent->subtree_root(to);
   if (to < key)
      return key;
   if (to > key)
      return key;
}

int static_net_t::process_request(int from, int to)
{
   if (net_nodes_.find(from) == net_nodes_.end() || net_nodes_.find(to) == net_nodes_.end())
      throw std::runtime_error("Incorrect from or to");

   return net_nodes_[from]->route(to);
}
