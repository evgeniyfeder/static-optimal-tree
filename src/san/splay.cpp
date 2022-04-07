#include "san/splay.h"

std::shared_ptr<net_node_t> splay::splay_net_t::left_rotate(std::shared_ptr<net_node_t> const & v)
{
   net_node_t * pp = p(v);
   std::shared_ptr<net_node_t> r = v->right;

   if (pp != nullptr)
   {
      if (pp->left && pp->left->key == v->key)
         pp->left = r;
      else
         pp->right = r;
   }
   else
   {
      root_ = r;
   }

   std::shared_ptr<net_node_t> tmp = r->left;
   r->left = v;
   v->right = tmp;

   v->parent = r.get();
   r->parent = pp;

   if (v->right)
   {
      v->right->parent = v.get();
      v->max_key = v->right->max_key;
   }
   else
   {
      v->max_key = v->key;
   }

   r->min_key = v->min_key;

   return r;
}

std::shared_ptr<net_node_t> splay::splay_net_t::right_rotate(std::shared_ptr<net_node_t> const & v)
{
   net_node_t * pp = p(v);
   std::shared_ptr<net_node_t> l = v->left;

   if (pp != nullptr)
   {
      if (pp->left->key == v->key)
         pp->left = l;
      else
         pp->right = l;
   }
   else
   {
      root_ = l;
   }

   std::shared_ptr<net_node_t> tmp = l->right;
   l->right = v;
   v->left = tmp;

   v->parent = l.get();
   l->parent = pp;

   if (v->left)
   {
      v->left->parent = v.get();
      v->min_key = v->left->min_key;
   }
   else
   {
      v->min_key = v->key;
   }

   l->max_key = v->max_key;

   return l;
}

net_node_t * splay::p(const std::shared_ptr<net_node_t> & root)
{
   return root->parent;
}

net_node_t * splay::g(const std::shared_ptr<net_node_t> & root)
{
   if (net_node_t * pp = p(root); pp)
      return pp->parent;
   return nullptr;
}

void splay::splay_net_t::splay_up(std::shared_ptr<net_node_t> & v, int subroot_key)
{
   bool is_subroot = v->key == subroot_key;

   while (!is_subroot)
   {
      if (p(v)->key == subroot_key || (g(v) && g(v)->key == subroot_key))
         is_subroot = true;

      if (p(v)->left && p(v)->left->key == v->key)
      {
         // zig
         if (g(v) == nullptr || p(v)->key == subroot_key)
         {
            right_rotate(p_or_root(v));
         }
         // zig-zig
         else if (g(v)->left && g(v)->left->key == p(v)->key)
         {
            right_rotate(g_or_root(v));
            right_rotate(p_or_root(v));
         }
         // zig-zag
         else
         {
            right_rotate(p_or_root(v));
            left_rotate(p_or_root(v));
         }
      }
      else
      {
         // zig
         if (g(v) == nullptr || p(v)->key == subroot_key)
         {
            left_rotate(p_or_root(v));
         }
         // zig-zig
         else if (g(v)->right && g(v)->right->key == p(v)->key)
         {
            left_rotate(g_or_root(v));
            left_rotate(p_or_root(v));
         }
         // zig-zag
         else
         {
            left_rotate(p_or_root(v));
            right_rotate(p_or_root(v));
         }
      }
   }
}

std::shared_ptr<net_node_t> splay::splay_net_t::p_or_root(std::shared_ptr<net_node_t> const & v)
{
   if (g(v))
   {
      if (v->parent->parent->left && v->parent->parent->left->key == v->parent->key)
         return v->parent->parent->left;
      else
         return v->parent->parent->right;
   }
   return root_;
}

std::shared_ptr<net_node_t> splay::splay_net_t::g_or_root(std::shared_ptr<net_node_t> const & v)
{
   if (g(v) && g(v)->parent)
   {
      if (v->parent->parent->parent->left &&
          v->parent->parent->parent->left->key == v->parent->parent->key)
         return v->parent->parent->parent->left;
      else
         return v->parent->parent->parent->right;
   }
   return root_;
}

splay::splay_net_t::splay_net_t(const std::shared_ptr<node_t> & tree_root) : net_t(tree_root) {}

int splay::splay_net_t::process_request(int from, int to)
{
   if (from == to)
      return 0;

   int result = net_nodes_[from]->route(to);

   splay_up(net_nodes_[from], net_nodes_[from]->subtree_root(to));
   if (from < to)
      splay_up(net_nodes_[to], net_nodes_[from]->right->key);
   else
      splay_up(net_nodes_[to], net_nodes_[from]->left->key);

   return result;
}
