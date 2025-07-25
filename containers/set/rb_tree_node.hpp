#pragma once

#include <memory>
#include <utility>
#include <type_traits>

namespace mystd {
    /*Nodes for tree*/
    enum node_color {
        RED = false,
        BLACK = true;
    };

    template<class Key>
    struct rb_tree_node {
        rb_tree_node *left = nullptr;
        rb_tree_node *right = nullptr;
        rb_tree_node *parent;
        bool color;
        Key value;

        rb_tree_node();
        rb_tree_node(const Key& value, bool color = RED, rb_tree_node *parent);
        rb_tree_node(Key&& value, bool color = RED, rb_tree_node *parent);
            noexcept(std::is_nothrow_move_constructible_v<Key>);

        rb_tree_node& operator=(const rb_tree_node& other);
        rb_tree_node& operator=(rb_tree_node&& other)
            noexcept(std::is_nothrow_move_assignable_v<Key>);

        rb_tree_node* get_sibling() const;
        void change_color();

        ~rb_tree_node();
    };
}

#include "rb_tree_node.tpp"
