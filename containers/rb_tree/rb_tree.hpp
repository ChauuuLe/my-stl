#pragma once

#include "rb_tree_node.hpp"
#include "../../ultilities/functional.hpp"
#include <utility>
#include <memory>
#include <type_traits>

namespace mystd {
    template<
        class Key,
        class Compare = mystd::less<Key>,
        class Allocator = std::allocator<Key>
    > struct rb_tree {
        using key_type = Key;
        using allocator_type = Allocator;
        using key_compare = Compare;
        using node_type = typename rb_tree_node<Key>;
        using size_type = size_t;

        node_type *root;
        node_type *max_node;
        node_type *min_node;
        node_type nil;

        allocator_type allocator;
        key_compare compare;

        rb_tree();
        explicit rb_tree(const key_compare& comp, const allocator_type& alloc = Allocator());
        explicit rb_tree(const allocator_type& alloc);
    
        std::pair<node_type*, bool> insert(const key_type& value);
        std::pair<node_type*, bool> insert(key_type&& value);
        size_type erase(const key_type& value);
        node_type* find(const key_type& value) const;

        node_type* find_max(node_type *node) const;
        node_type* find_min(node_type *node) const;
        node_type* find_max() const;
        node_type* find_min() const;
        node_type* lower_bound(const key_type& value) const;
        node_type* upper_bound(const key_type& value) const;

        node_type* make_node(const key_type& value, bool is_left, bool color, node_type *parent = nullptr);
        node_type* make_node(key_type&& value, bool is_left, bool color, node_type *parent = nullptr);
        void init_nil();

        bool is_black(node_type *node);
        void flip_color(node_type *node);
        void rotate_left(node_type *parent);
        void rotate_right(node_type *parent);
        void transplant(node_type *u, node_type* v);

        void fix_up_insert(node_type *node, bool is_left);
        void fix_up_delete(node_type *node);

        void free_node(node_type *node);
    };
}

#include "rb_tree.tpp"
