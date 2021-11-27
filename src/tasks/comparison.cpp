#include "tasks.h"

#include <net.h>
#include <san/period_optimal_net.h>

#include <random>
#include <chrono>


std::vector<compare_result> compare_nets(std::size_t n, std::size_t m, std::size_t req_bar)
{
   pair_list_t requests;

   std::size_t i = 0;
   requests.reserve(m);

   std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
   std::uniform_int_distribution<int> dist(0, n - 1);
   while (i < m)
   {
      std::size_t from = dist(rng) % n, to = dist(rng) % n;
      if (from != to)
      {
         requests.push_back({from, to});
         ++i;
      }
   }

   // networks to compare
   static_net_t uniform_net{make_uniform_three_net(n)};
   std::size_t cost_uniform_net = 0;

   period_optimal_net_t po_net{adjacency_matrix_t{n, 1}};
   std::size_t cost_po_net = 0;

   adjacency_matrix_t cur_matrix(n, 0);

   std::vector<compare_result> result;
   for (std::size_t j = 1; j < m; j++)
   {
      int from = requests[j].first, to = requests[j].second;

      cur_matrix.add_request(from, to);

      cost_uniform_net += uniform_net.process_request(from, to);
      cost_po_net += po_net.process_request(from, to);

      if (j % req_bar == 0)
      {
         auto opt_info = make_optimal_net(cur_matrix);

         compare_result cur = {0};
         cur.step = j;
         cur.period_optimal = static_cast<double>(cost_po_net) / j;
         cur.static_optimal = static_cast<double>(opt_info.cost) / j;
         cur.uniform_optimal = static_cast<double>(cost_uniform_net) / j;

         result.emplace_back(cur);
      }
   }
   return result;
}

int main(int argc, char *argv[])
{
   std::size_t n = 100, m = 100000, bar = 1000;
   auto result = compare_nets(n, m, bar);

   std::cout << "step,optimal,uniform,period" << std::endl;
   for (auto const & res : result)
   {
      std::cout << res.step << "," << res.static_optimal << "," << res.uniform_optimal << "," << res.period_optimal << std::endl;
   }
}