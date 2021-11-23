#include "net.h"

#include <gtest/gtest.h>

TEST(net, calc_route)
{
   std::shared_ptr<node_t> tree = make_uniform_squared_net(7);
   // tree structure. easily to setup via make_uniform_squared_net
   //   0
   //    1
   //        2
   //      3
   //          4
   //        5
   //          6

   static_net_t net(tree);
   ASSERT_EQ(net.process_request(3, 0), 2);
   ASSERT_EQ(net.process_request(6, 2), 3);
   ASSERT_EQ(net.process_request(1, 4), 3);
}

