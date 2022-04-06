#include "net.h"

namespace splay
{
   std::shared_ptr<net_node_t> left_rotate(std::shared_ptr<net_node_t> const & root);
   std::shared_ptr<net_node_t> right_rotate(std::shared_ptr<net_node_t> const & root);

   std::shared_ptr<net_node_t> splay_up(std::shared_ptr<net_node_t> const & x, std::int32_t to);
}
