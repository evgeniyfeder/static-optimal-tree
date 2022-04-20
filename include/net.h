#pragma once

#include "requests.h"
#include "tree.h"

#include <memory>
#include <utility>
#include <iostream>
#include <unordered_map>

struct net_node_t
{
   int key;
   std::shared_ptr<net_node_t> left, right;
   net_node_t * parent = nullptr;
   int min_key, max_key;

   explicit net_node_t(node_t const & node);
   int route(int to);

   int subtree_root(int to);

   void to_line_rec(std::vector<int> & result)
   {
      result.push_back(key);

      if (left)
         left->to_line_rec(result);

      if (right)
         right->to_line_rec(result);
   }
};

class net_t
{
public:
   net_t(std::shared_ptr<node_t> const & tree_root);
   net_t(std::shared_ptr<net_node_t> const & tree_root);

   virtual int process_request(int from, int to) = 0;

   std::shared_ptr<net_node_t> root() const { return root_; }

protected:
   void fill_net_nodes(std::shared_ptr<net_node_t> const & cur_node);

protected:
   std::unordered_map<int, std::shared_ptr<net_node_t>> net_nodes_;
   std::shared_ptr<net_node_t> root_;
};

class static_net_t : public net_t
{
public:
   explicit static_net_t(std::shared_ptr<node_t> const & tree_root) : net_t(tree_root) {};
   int process_request(int from, int to) override;
};

struct static_optimal_tree_t
{
   std::uint64_t cost = 0;
   std::shared_ptr<node_t> opt_root;

   static_optimal_tree_t & operator=(static_optimal_tree_t const & other) = default;

   static_optimal_tree_t() = default;

   static_optimal_tree_t(uint64_t cost, std::shared_ptr<node_t> opt_root) : cost(cost), opt_root(std::move(opt_root)) {}
};

static_optimal_tree_t make_optimal_net(adjacency_matrix_t const & m);
std::shared_ptr<node_t> make_uniform_squared_net(std::size_t n);
std::shared_ptr<node_t> make_full_tree(std::size_t n);
std::shared_ptr<node_t> make_uniform_three_net(std::size_t n);
