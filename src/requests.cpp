#include "requests.h"
#include "net.h"

#include <iostream>

adjacency_matrix_t::adjacency_matrix_t(pair_list_t const & requests, std::size_t n)
      : m_(n, std::vector<int>(n, 0)), n_(n)
{
   for (auto && req: requests)
   {
      m_[req.first][req.second]++;
      m_[req.second][req.first]++;
   }

   for (std::size_t i = 0; i < n; ++i)
      m_[i][i] = 0;
}

adjacency_matrix_t::adjacency_matrix_t(std::size_t n, int m)
      : m_(n, std::vector<int>(n, m)), n_(n)
{
   for (std::size_t i = 0; i < n; ++i)
      m_[i][i] = 0;
}

void adjacency_matrix_t::print() const noexcept
{
   std::cout << "===================\n";
   std::cout << "Adjancency matrix: \n";
   for (auto const & row : m_)
   {
      for (auto const & x: row)
      {
         std::cout << x << " ";
      }
      std::cout << std::endl;
   }
   std::cout << "===================\n";
}

std::vector<int> & adjacency_matrix_t::operator[](std::size_t i)
{
   return m_[i];
}

std::size_t adjacency_matrix_t::size() const noexcept
{
   return m_.size();
}

std::vector<int> const & adjacency_matrix_t::operator[](std::size_t i) const
{
   return m_[i];
}

void fill_vector(std::shared_ptr<node_t> const & node, std::vector<std::shared_ptr<node_t>> & nodes)
{
   nodes[node->key] = node;
   if (node->left)
      fill_vector(node->left, nodes);
   if (node->right)
      fill_vector(node->right, nodes);
}

// invariant: all nodes 0 <= key < n_
std::uint64_t adjacency_matrix_t::process_requests(std::shared_ptr<node_t> const & tree_root)
{
   static_net_t net(tree_root);

   std::uint64_t result = 0;

   // out matrix is simmetric so u->v or v->u doesn't matter
   for (std::size_t i = 0; i < n_; ++i)
      for (std::size_t j = i + 1; j < n_; ++j)
      {
         for (std::size_t k = 0; k < m_[i][j]; ++k)
            result += net.process_request(i, j);
      }

   return result;
}

void adjacency_matrix_t::add_request(int from, int to)
{
   m_[from][to]++;
   m_[to][from]++;
}

std::size_t count_num_full_tree(std::size_t h)
{
   // sum((2 ^(n - 1 - i)) * (2^(i + 1) - 1) * ((2^n) - 1 - (2^(i + 1) - 1)), i=0, i=n-2)
   return (1 << h) * ((1 << h) * h + h - 3 * (1 << h) + 3);
}


