#pragma once

#include <cstdint>
#include <vector>
#include <memory>

using pair_list_t = std::vector<std::pair<std::int32_t, std::int32_t>>;

struct node_t;

class adjacency_matrix_t
{
public:
   adjacency_matrix_t(pair_list_t const & requests, std::size_t n);
   adjacency_matrix_t(std::size_t n, int m);

   std::vector<int> & operator[](std::size_t i);
   std::vector<int> const & operator[](std::size_t i) const;

   std::size_t size() const noexcept;
   void print() const noexcept;

   void add_request(int from, int to);

   std::uint64_t process_requests(std::shared_ptr<node_t> const & root);
private:
   std::vector<std::vector<int>> m_;
   std::size_t n_;
};

std::size_t count_num_full_tree(std::size_t h);
std::size_t count_num_squared_uniform_tree(std::size_t n);
