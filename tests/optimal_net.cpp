#include "net.h"

#include <gtest/gtest.h>

TEST(optimal_tree, check_dynamic_result_with_tree_structure)
{
   for (std::size_t n = 10; n < 50; ++n)
   {
      auto m = adjacency_matrix_t(n, 1);
      static_optimal_tree_t tree = make_optimal_net(m);

      ASSERT_EQ(m.process_requests(tree.opt_root), tree.cost);
   }
}

