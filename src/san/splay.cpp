#include "san/splay.h"

std::shared_ptr<net_node_t> left_rotate(std::shared_ptr<net_node_t> const & root)
{
   std::shared_ptr<net_node_t> x = root->right;
   root->right = x->left;
   x->left = root;

   x->parent = root->parent;
   root->parent = x.get();

   if (root->right)
   {
      root->right->parent = root.get();
      root->max_key = root->right->max_key;
   }
   else
   {
      root->max_key = root->key;
   }

   x->min_key = root->min_key;

   return x;
}

std::shared_ptr<net_node_t> splay::right_rotate(std::shared_ptr<net_node_t> const & root)
{
   std::shared_ptr<net_node_t> x = root->left;

   root->left = x->right;
   x->right = root;

   x->parent = root->parent;
   root->parent = x.get();

   if (root->left)
   {
      root->left->parent = root.get();
      root->min_key = root->left->min_key;
   }
   else
   {
      root->min_key = root->key;
   }

   x->max_key = root->max_key;

   return x;
}

std::shared_ptr<net_node_t> splay::splay_up(std::shared_ptr<net_node_t> const & x, std::int32_t to)
{
   net_node_t * parent = x->parent;
   if (!parent)
      return x;

   if (x->)

}