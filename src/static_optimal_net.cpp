#include "requests.h"
#include "net.h"

#include <cassert>
#include <limits>


// based on https://eprints.cs.univie.ac.at/5454/1/13_SplayNet.pdf section III.A
// input invariant: m[i][i] == 0
static_optimal_tree_t make_optimal_net(adjacency_matrix_t const & m)
{

   assert(m.size() == m[0].size());
   std::size_t n = m.size();

   // count prefix sums for count W_I(v)
   adjacency_matrix_t pref_sum(n, 0);
   for (std::size_t from = 0; from < n; ++from)
   {
      pref_sum[from][0] = m[from][0];
      for (std::size_t to = 1; to < n; ++to)
         pref_sum[from][to] = pref_sum[from][to - 1] + m[from][to];
   }

   // Init dp array and for each vertex set start of dynamic
   std::vector<std::vector<static_optimal_tree_t>> dp;
   dp.resize(n + 1);
   for (std::size_t i = 0; i < n; ++i)
   {
      dp[i].resize(n + 1);
      static_optimal_tree_t init(0, std::make_shared<node_t>(i));
      dp[i][i + 1] = init;
   }
   dp[n].resize(n + 1);
   for (std::size_t size = 2; size <= n; ++size)
   {
      for (std::size_t left = 0; left < n; ++left)
      {
         if (left + size > n)
            continue;

         // Precalculate sum of W_I(v)
         std::uint64_t W_I_left = 0, W_I_right = 0;
         for (std::size_t v = left + 1; v < left + size; v++)
         {
            W_I_right += pref_sum[v][left] + (pref_sum[v][n - 1] - pref_sum[v][left + size - 1]);
         }


         static_optimal_tree_t min_tree(std::numeric_limits<std::uint64_t>::max(), nullptr);

         for (std::size_t mid = left; mid < left + size; mid++)
         {
            std::size_t lt_start = left, lt_finish = mid, rt_start = mid + 1, rt_finish = left + size;

            std::size_t cost = dp[lt_start][lt_finish].cost + dp[rt_start][rt_finish].cost + W_I_left + W_I_right;

            if (cost < min_tree.cost)
            {
               min_tree = static_optimal_tree_t(cost, std::make_shared<node_t>(mid, dp[lt_start][lt_finish].opt_root,
                                                                               dp[rt_start][rt_finish].opt_root));
            }

            // Update W_I
            W_I_left += (left > 0 ? pref_sum[mid][left - 1] : 0) + (pref_sum[mid][n - 1] - pref_sum[mid][mid])
                        - ((mid > 0 ? pref_sum[mid][mid - 1] : 0)
                           - (left > 0 ? pref_sum[mid][left - 1] : 0));

            if (mid != n - 1)
               W_I_right -= pref_sum[mid + 1][mid] + (pref_sum[mid + 1][n - 1] - pref_sum[mid + 1][left + size - 1])
                            - (pref_sum[mid + 1][left + size - 1] - pref_sum[mid + 1][mid]);

         }
         dp[left][left + size] = min_tree;
      }
   }
   return dp[0][n];
}

