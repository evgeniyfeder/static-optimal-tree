#include "net.h"

namespace splay
{
   net_node_t * p(std::shared_ptr<net_node_t> const & root);
   net_node_t * g(std::shared_ptr<net_node_t> const & root);

   class splay_net_t : public net_t
   {
   public:
      explicit splay_net_t(std::shared_ptr<node_t> const & tree_root);
      int process_request(int from, int to) override;
   private:

      std::shared_ptr<net_node_t> left_rotate(std::shared_ptr<net_node_t> const & v);
      std::shared_ptr<net_node_t> right_rotate(std::shared_ptr<net_node_t> const & v);

      std::shared_ptr<net_node_t> p_or_root(std::shared_ptr<net_node_t> const & v);
      std::shared_ptr<net_node_t> g_or_root(std::shared_ptr<net_node_t> const & root);

      void splay_up(std::shared_ptr<net_node_t> & x);
   };
}

