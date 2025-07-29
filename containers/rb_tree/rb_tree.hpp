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
        using base_node_type = rb_tree_node_base;
        using size_type = size_t;

        /*
        Root is header parent
        Right is max node
        Left is min node
        */
        base_node_type header;

        allocator_type allocator;
        key_compare compare;

        rb_tree();
        explicit rb_tree(const key_compare& comp, const allocator_type& alloc = Allocator());
        explicit rb_tree(const allocator_type& alloc);
    
        std::pair<base_node_type*, bool> insert(const key_type& value);
        std::pair<base_node_type*, bool> insert(key_type&& value);
        size_type erase(const key_type& value);
        base_node_type* find(const key_type& value) const;

        base_node_type* find_max(base_node_type *node) const;
        base_node_type* find_min(base_node_type *node) const;
        base_node_type* find_max() const;
        base_node_type* find_min() const;
        base_node_type* lower_bound(const key_type& value) const;
        base_node_type* upper_bound(const key_type& value) const;

        base_node_type* make_node(const key_type& value, bool is_left, bool color, base_node_type *parent = nullptr);
        base_node_type* make_node(key_type&& value, bool is_left, bool color, base_node_type *parent = nullptr);
        void init_nil();

        bool is_black(base_node_type *node);
        void flip_color(base_node_type *node);
        void rotate_left(base_node_type *parent);
        void rotate_right(base_node_type *parent);
        void transplant(base_node_type *u, base_node_type* v);

        void fix_up_insert(base_node_type *node, bool is_left);
        void fix_up_delete(base_node_type *node);

        void free_node(base_node_type *node);
    };
}

#include "rb_tree.tpp"
