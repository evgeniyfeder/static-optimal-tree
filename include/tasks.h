#pragma once

#include <vector>

struct compare_result
{
   std::size_t step;
   double static_optimal, uniform_optimal, period_optimal;
};

std::vector<compare_result> compare_nets(std::size_t n, std::size_t m, std::size_t req_bar = 10);