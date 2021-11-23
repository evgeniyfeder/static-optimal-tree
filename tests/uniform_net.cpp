#include "net.h"
#include "requests.h"

#include "gtest/gtest.h"

void tree_equality(std::shared_ptr<node_t> const & a, std::shared_ptr<node_t> const & b)
{
   if (!a && !b)
      return;

   ASSERT_FALSE((a && !b) || (!a && b));

   ASSERT_EQ(a->key, b->key);
   tree_equality(a->left, b->left);
   tree_equality(a->right, b->right);
}


TEST(uniform_tree_squared, build_10)
{
   std::size_t n = 10;
   auto tree = make_uniform_squared_net(n);
   std::vector<int> line_tree = tree->to_line(), res = {0, 2, 1, 6, 4, 3, 5, 8, 7, 9};
   ASSERT_EQ(line_tree, res);
}

TEST(uniform_tree_squared, eq_with_optimal_balanced)
{
   for (std::size_t i = 4; i < 100; i++)
   {
      auto uni_tree = make_uniform_squared_net(i);

      auto matrix = adjacency_matrix_t(i, 1);
      static_optimal_tree_t opt_tree = make_optimal_net(matrix);
      auto uni_line = uni_tree->to_line(), opt_line = opt_tree.opt_root->to_line();

      ASSERT_EQ(matrix.process_requests(uni_tree), opt_tree.cost);
      ASSERT_EQ(uni_line, opt_line);
   }
}

TEST(uniform_tree_half, build_10)
{
   std::size_t n = 10;
   auto tree = make_full_tree(n);
   std::vector<int> line_tree = tree->to_line(), res = { 6, 3, 1, 0, 2, 5, 4, 8, 7, 9 };
   ASSERT_EQ(line_tree, res);
}

TEST(uniform_tree_three, eq_with_optimal_balanced)
{
   for (std::size_t i = 5; i < 100; i++)
   {
      auto squared_tree = make_uniform_squared_net(i);

      auto matrix = adjacency_matrix_t(i, 1);
      static_optimal_tree_t opt_tree = make_optimal_net(matrix);

      ASSERT_EQ(matrix.process_requests(squared_tree), opt_tree.cost);
   }
}

TEST(uniform_tree, full_tree_count_number)
{
   for (std::size_t i = 2; i < 100; i++)
   {
      std::size_t n = (1 << i) - 1;
      auto matrix = adjacency_matrix_t(n, 1);
      ASSERT_EQ(matrix.process_requests(make_full_tree(n)), count_num_full_tree(i));
   }
}
