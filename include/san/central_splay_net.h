#pragma once

#include "net.h"
#include "requests.h"
#include "splay.h"

class central_splay_net : public net_t
{
public:
   explicit central_splay_net(std::shared_ptr<node_t> const & tree_root, std::shared_ptr<node_t> const & central_node);
   int process_request(int from, int to) override;
private:

   std::shared_ptr<node_t> central_node;
};