#pragma once

#include <iostream>

static const int PADDING_SIZE = 2;

struct line_tag_t {};
struct tree_tag_t {};

struct node_t
{
   int key;
   std::shared_ptr<node_t> left, right;

   node_t(int key,
          std::shared_ptr<node_t> left  = nullptr,
          std::shared_ptr<node_t> right = nullptr)
         : key(key), left(left), right(right) {}

   void print(std::size_t padding, tree_tag_t)
   {
      if (right)
         right->print(padding + PADDING_SIZE, tree_tag_t{});

      for (std::size_t i = 0; i < padding; ++i)
         std::cout << " ";
      std::cout << key << std::endl;

      if (left)
         left->print(padding + PADDING_SIZE, tree_tag_t{});

   };

   bool operator==(node_t const & rhs) const
   {
      bool result = true;

      if (key != rhs.key)
         return false;

      if (left && rhs.left)
         result = result && (*left == *rhs.left);
      else if (!left && !rhs.left)
         result = result;
      else if (!left || !rhs.left)
         result = false;

      if (right && rhs.right)
         result = result && *right == *rhs.right;
      else if (!right && !rhs.right)
         result = result;
      else if (!right || !rhs.right)
         result = false;

      return result;
   }

   bool operator!=(node_t const & rhs) const
   {
      return !(rhs == *this);
   }

   void print(line_tag_t)
   {
      std::cout << key << " ";

      if (left)
         left->print(line_tag_t{});

      if (right)
         right->print(line_tag_t{});
   }
private:
   void to_line_rec(std::vector<int> & result)
   {
      result.push_back(key);

      if (left)
         left->to_line_rec(result);

      if (right)
         right->to_line_rec(result);
   }

public:
   std::vector<int> to_line()
   {
      std::vector<int> res;
      to_line_rec(res);
      return res;
   }
};
