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
        allocator_type Allocator;
        key_compare compare;

        rb_tree();
        explicit rb_tree(const key_compare& comp, const allocator_type& alloc = Allocator());
        explicit rb_tree(const allocator_type& alloc);
    
        std::pair<node_type*, bool> insert(const key_type& value);
        std::pair<node_type*, bool> insert(key_type&& value);

        size_type erase(const key_type& key);
        node_type* find(const key_type& key);

        void make_node(node_type *node, const key_type& value, bool is_left, bool color = RED, node_type *parent = nullptr);
        void rotate_node_color(node_type *node);
        void rotate_left(node_type *parent, node_type *right_child);
        void rotate_right(node_type *parent, node_type *left_child);

        void fix_insert(node_type *node, bool is_left);
    };
}

#include "rb_tree.tpp"
