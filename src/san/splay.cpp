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
   r->left  = v;
   v->right = tmp;

   v->parent = r.get();
   r->parent = pp;

   if (v->right)
   {
      v->right->parent = v.get();
      v->max_key       = v->right->max_key;
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
      if (pp->left && pp->left->key == v->key)
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
   v->left  = tmp;

   v->parent = l.get();
   l->parent = pp;

   if (v->left)
   {
      v->left->parent = v.get();
      v->min_key      = v->left->min_key;
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

std::size_t count_subtree_size(net_node_t * node, std::unordered_map<int, std::size_t> & sizes)
{
   if (!node->left && !node->right)
   {
      sizes[node->key] = 1;
      return sizes[node->key];
   }

   std::size_t result = 0;
   if (node->left)
      result += count_subtree_size(node->left.get(), sizes);
   if (node->right)
      result += count_subtree_size(node->right.get(), sizes);

   sizes[node->key] = result;
   return result;
}

std::shared_ptr<net_node_t> find_centroid(const std::shared_ptr<net_node_t> & v,
                                          std::unordered_map<int, std::size_t> const & sizes,
                                          std::size_t n)
{
   if (v->left && sizes.at(v->left->key) > double(n) / 2)
      return find_centroid(v->left, sizes, n);
   else if (v->right && sizes.at(v->right->key) > double(n) / 2)
      return find_centroid(v->right, sizes, n);
   else
      return v;
}

std::shared_ptr<net_node_t> splay::find_centroid(const std::shared_ptr<net_node_t> & v)
{
   std::unordered_map<int, std::size_t> sizes;
   count_subtree_size(v.get(), sizes);

   return find_centroid(v, sizes, sizes[v->key]);
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

splay::splay_net_t::splay_net_t(const std::shared_ptr<node_t> & tree_root)
   : net_t(tree_root)
{}

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

splay::splay_net_t::splay_net_t(std::shared_ptr<net_node_t> const & tree_root)
   : net_t(tree_root)
{

}


bool splay::splay_net_t::in_range(int key) const
{
   return root_->min_key <= key && key <= root_->max_key;
}

void update_range(net_node_t * node)
{
   if (node == nullptr)
      return;
   if (node->left && node->min_key != node->left->min_key)
      node->min_key = node->left->min_key;
   if (node->right && node->max_key != node->right->max_key)
      node->max_key = node->right->max_key;
   update_range(node->parent);
}

splay::central_splay_net_t::central_splay_net_t(std::shared_ptr<node_t> const & tree_root)
   : left(std::shared_ptr<node_t>{}), right(std::shared_ptr<node_t>{}), parent(std::shared_ptr<node_t>{})
{
   auto net_root = std::make_shared<net_node_t>(*tree_root);
   net_center_ = find_centroid(net_root);

   left = splay_net_t{net_center_->left};
   right = splay_net_t{net_center_->right};

   if (net_center_->parent)
   {
      if (net_center_->key > net_center_->parent->key)
      {
         net_center_->parent->right   = nullptr;
         net_center_->parent->max_key = net_center_->parent->key;
      }
      else
      {
         net_center_->parent->left   = nullptr;
         net_center_->parent->min_key = net_center_->parent->key;
      }
      update_range(net_center_->parent->parent);
      net_center_->parent = nullptr;
   }

   net_center_->left->parent = nullptr;
   net_center_->left = nullptr;

   net_center_->right->parent = nullptr;
   net_center_->right = nullptr;

   parent = splay_net_t{net_root};
}

int splay::central_splay_net_t::process_request(int from, int to)
{
   if (from == to)
      return 0;

   // they are from one subtree
   if (left.in_range(from) && left.in_range(to))
      return left.process_request(from, to);
   if (right.in_range(from) && right.in_range(to))
      return right.process_request(from, to);
   if (parent.in_range(from) && parent.in_range(to))
      return parent.process_request(from, to);

   // one of nodes is center node
   if (from == net_center_->key)
   {
      if (left.in_range(to))
         return left.process_request(left.root_->key, to) + 1;
      if (right.in_range(to))
         return right.process_request(right.root_->key, to) + 1;
      if (parent.in_range(to))
         return parent.process_request(parent.root_->key, to) + 1;
   }
   if (to == net_center_->key)
   {
      if (left.in_range(from))
         return left.process_request(from, left.root_->key) + 1;
      if (right.in_range(from))
         return right.process_request(from, right.root_->key) + 1;
      if (parent.in_range(from))
         return parent.process_request(from, parent.root_->key) + 1;
   }

   if (left.in_range(from))
   {
      if (right.in_range(to))
         return left.process_request(from, left.root_->key) + 1 + right.process_request(right.root_->key, to);
      if (parent.in_range(to))
         return left.process_request(from, left.root_->key) + 1 + parent.process_request(parent.root_->key, to);
   }

   if (right.in_range(from))
   {
      if (left.in_range(to))
         return right.process_request(from, right.root_->key) + 1 + left.process_request(left.root_->key, to);
      if (parent.in_range(to))
         return right.process_request(from, right.root_->key) + 1 + parent.process_request(parent.root_->key, to);
   }

   if (parent.in_range(from))
   {
      if (left.in_range(to))
         return parent.process_request(from, parent.root_->key) + 1 + left.process_request(left.root_->key, to);
      if (right.in_range(to))
         return parent.process_request(from, parent.root_->key) + 1 + right.process_request(right.root_->key, to);
   }
   static_assert("All cases");
}