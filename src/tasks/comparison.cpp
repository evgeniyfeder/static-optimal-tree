#include "tasks.h"

#include <net.h>
#include <san/period_optimal_net.h>

#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <filesystem>

std::pair<pair_list_t, std::size_t> load_requests(std::filesystem::path const & input_path)
{
   std::ifstream input_f(input_path);

   std::size_t n, m;
   input_f >> n >> m;

   pair_list_t result(m);
   for (std::size_t i = 0; i < m; i++)
   {
      input_f >> result[i].first >> result[i].second;
   }
   return {result, n};
}

std::vector<compare_result> compare_nets(std::size_t n, pair_list_t const & requests, std::size_t req_bar)
{;
   std::size_t m = requests.size();
   // dump requests as an input to splaynet execution for comparing results with splaynet
   std::ofstream req_file("requests.in");

   req_file << n << std::endl;

   std::vector<int> nodes(n);
   for (int i = 0; i < n; i++)
      nodes[i] = i;

   std::shuffle(nodes.begin(), nodes.end(), std::mt19937(std::random_device()()));

   for (int i = 0; i < n; i++)
      req_file << nodes[i] << " ";
   req_file << std::endl;

   req_file << 1 /* alpha */ << std::endl << m << std::endl;
   for (std::size_t j = 0; j < m; j++)
   {
      int from = requests[j].first, to = requests[j].second;

      req_file << from << " " << to << std::endl;
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
      std::cout << j << std::endl;
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
   std::vector<compare_result> result;


   if (argc == 1)
   {
      std::cout << "Generating uniform workload..." << std::endl;
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
      result = compare_nets(n, requests, bar);
   }
   else if (argc == 2)
   {
      auto const & [requests, n] = load_requests(argv[1]);
      std::cout << "Using input from the file" << std::endl;
      result = compare_nets(n, requests, bar);
   }

   std::ofstream stat_f("stat2.csv");
   stat_f << "step,optimal,uniform,period" << std::endl;
   for (auto const & res : result)
   {
      stat_f << res.step << "," << res.static_optimal << "," << res.uniform_optimal << "," << res.period_optimal << std::endl;
   }
}