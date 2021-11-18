#include "net.h"
#include "requests.h"

#include <chrono>
#include <iostream>
#include <random>

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      std::cout << "Usage: main N M" << std::endl;
      return 0;
   }

   std::size_t n = std::atoi(argv[1]), m = std::atoi(argv[2]);
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

   adjacency_matrix_t matrix = adjacency_matrix_t(n, m);// make_adjacency_matrix(requests, n);

   static_optimal_tree_t result = make_optimal_net(matrix);
   std::cout << n << " " << result.cost << std::endl;

   freopen("a.tree", "w", stdout);
   result.opt_root->print(line_tag_t{});
   return 0;
}