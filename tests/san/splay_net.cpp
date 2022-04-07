#include "san/splay.h"

#include <gtest/gtest.h>

#include <vector>

TEST(splay_net, root_lca_requests)
{
   splay::splay_net_t net(make_full_tree(7));

   ASSERT_EQ(4, net.process_request(0, 4));

   std::vector<int> net_line, expected = {4, 1, 0, 3, 2, 5, 6};
   net.root()->to_line_rec(net_line);

   ASSERT_EQ(net_line, expected);

   ASSERT_EQ(4, net.process_request(3, 6));

   net_line.clear();
   expected = {6, 3, 1, 0, 2, 5, 4};
   net.root()->to_line_rec(net_line);

   ASSERT_EQ(net_line, expected);

}