#include "san/splay.h"

#include <gtest/gtest.h>

#include <vector>

void test(splay::splay_net_t & net, int from, int to, int len, std::vector<int> const & expected)
{
   ASSERT_EQ(len, net.process_request(from, to));

   std::vector<int> net_line;
   net.root()->to_line_rec(net_line);

   ASSERT_EQ(net_line, expected);
}

TEST(splay_net, root_lca_requests)
{
   splay::splay_net_t net(make_full_tree(7));

   test(net, 0, 4, 4, {0, 4, 1, 3, 2, 5, 6});
   test(net, 0, 6, 3, {0, 6, 5, 4, 1, 3, 2});
}

TEST(splay_net, subroot_lca_requests)
{
   splay::splay_net_t net(make_full_tree(10));

   test(net, 0, 4, 4, {6, 0, 4, 1, 3, 2, 5, 8, 7, 9});
   test(net, 8, 9, 1, {6, 0, 4, 1, 3, 2, 5, 8, 7, 9});
}