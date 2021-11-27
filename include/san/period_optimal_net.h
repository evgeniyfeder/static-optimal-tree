#pragma once

#include "net.h"
#include "requests.h"

class period_optimal_net_t : public net_t
{
public:
   explicit period_optimal_net_t(adjacency_matrix_t m);
   int process_request(int from, int to) override;
private:
   std::size_t counter;
   adjacency_matrix_t m_;
};