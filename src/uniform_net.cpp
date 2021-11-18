#include "net.h"

#include <cassert>
#include <langinfo.h>


std::size_t maxbit(std::size_t n)
{
   std::size_t res = 0;
   while (n != 1)
   {
      res++;
      n = n >> 1;
   }
   return res;
}

namespace square_impl
{
   // [prefix ... prefix + (1 << depth) - 2] with root prefix + (1 << (depth - 1)) - 1
   std::shared_ptr<node_t> build_full_tree(std::size_t depth, std::size_t prefix)
   {
      if (depth == 1)
         return std::make_shared<node_t>(prefix);

      auto left_subtree = build_full_tree(depth - 1, prefix);
      auto right_subtree = build_full_tree(depth - 1, prefix + (1 << (depth - 1)));

      return std::make_shared<node_t>(prefix + (1 << (depth - 1)) - 1, left_subtree, right_subtree);
   }

   // build tree for first n nodes
   std::pair<std::shared_ptr<node_t>, std::shared_ptr<node_t>>
   build_up_tree(std::size_t n)
   {
      if (n == 1)
      {
         auto root = std::make_shared<node_t>(0);
         return {root, root};
      }

      std::size_t mb = maxbit(n);

      std::size_t height_bottom = (n & (1 << (mb - 1))) != 0 ? mb : mb - 1;

      assert(n >= (1 << height_bottom));

      auto const &[up_res, tree_root] = build_up_tree(n - 1 - ((1 << height_bottom) - 1));

      std::shared_ptr<node_t> left_res;
      if (height_bottom != 0)
         left_res = build_full_tree(height_bottom, n - 1 - ((1 << height_bottom) - 1));

      std::shared_ptr<node_t> res = std::make_shared<node_t>(n - 1, left_res);

      up_res->right = res;
      return {res, tree_root};
   }
}
// build tree for uniform distribution of requests
//   tree_up
//        \
//        root
//      /      \
//  tree_left  tree_right
std::shared_ptr<node_t> make_uniform_squared_net(std::size_t n)
{
   std::shared_ptr<node_t> tree;

   n = n + 2;

   std::size_t mb = maxbit(n), i;


   if ((n & (1 << (mb - 1))) != 0)
   {
      i = mb - 1;
   }
   else
   {
      i = mb - 2;
   }

   std::size_t start = (1 << i) - 1;
   std::array<std::size_t, 3> sizes = {start, start, start};

   std::size_t cn = n - 2 - 3 * start, ind = 0;

   while (cn > 1 + (1 << i))
   {
      sizes[ind] += (1 << i);
      cn -= (1 << i);
      ind = (ind + 1) % 3;
   }

   std::size_t up_size = sizes[2] + cn - 1, left_size = sizes[1], right_size = sizes[0];


   // left and right need to be always full tree
   assert(left_size + 1 == 1 << maxbit(left_size + 1));
   assert(right_size + 1 == 1 << maxbit(right_size + 1));

   // build tree_up
   auto const & [up_tree, root] = square_impl::build_up_tree(up_size);
   auto left = square_impl::build_full_tree(maxbit(left_size + 1), up_size);
   auto right = square_impl::build_full_tree(maxbit(right_size + 1), up_size + left_size + 1);

   auto node = std::make_shared<node_t>(up_size + left_size, left, right);
   up_tree->right = node;

   return root;
}


namespace half_impl
{
   std::shared_ptr<node_t>
   make_down_tree(std::size_t l, std::size_t r) // [l, r)
   {
      if (l == r - 1)
         return std::make_shared<node_t>(l);
      if (l > r - 1)
         return nullptr;

      std::size_t med = (l + r) / 2;

      return std::make_shared<node_t>(med, make_down_tree(l, med), make_down_tree(med + 1, r));
   }

   std::pair<std::shared_ptr<node_t>, std::shared_ptr<node_t>>
   make_up_tree(std::size_t n)
   {
      if (n == 0)
      {
         auto root = std::make_shared<node_t>(0);
         return {root, root};
      }
      if (n == 1)
      {
         auto root = std::make_shared<node_t>(0);
         auto subroot = std::make_shared<node_t>(1);
         root->right = subroot;
         return {subroot, root};
      }

      auto const & [up_node, root] = make_up_tree(n / 2 - 1);
      std::shared_ptr<node_t> left = make_down_tree(n / 2, n);

      auto cur_node = std::make_shared<node_t>(n, left);

      up_node->right = cur_node;
      return {cur_node, root};
   }

}


// make net that constructing by the following idea
// we take an edge and devide nodes into two sets
std::shared_ptr<node_t> make_uniform_three_net(std::size_t n)
{
   //auto bottom = half_impl::make_down_tree(n / 2, n);
   auto const & [up_node, root] = half_impl::make_up_tree(n / 3 - 1);
   auto left = half_impl::make_down_tree(n / 3, 2 * n / 3);
   auto right = half_impl::make_down_tree(2 * n / 3 + 1, n);

   up_node->right = std::make_shared<node_t>(2 * n / 3, left, right);
   return root;
}

// make net that constructing by the following idea
// we take an edge and devide nodes into two sets
std::shared_ptr<node_t> make_uniform_half_net(std::size_t n)
{
   auto bottom = half_impl::make_down_tree(0, n);
   //auto const & [up_node, root] = half_impl::make_up_tree(n / 2 - 1);
   //up_node->right = bottom;
   return bottom;
}

