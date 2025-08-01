#pragma once

#include <memory>
#include <utility>
#include <type_traits>

namespace mystd {
    /*Nodes for tree*/
    enum class node_color : bool {
        RED = false,
        BLACK = true
    };

    struct rb_tree_node_base {
        bool color = BLACK;
        rb_tree_node_base *left = nullptr;
        rb_tree_node_base *right = nullptr;
        rb_tree_node_base *parent = nullptr;
    };

    template<class Key>
    struct rb_tree_node: public rb_tree_node_base {
        Key value;

        rb_tree_node();

        template<class... Args>
        rb_tree_node(Args&&... value_args)
            noexcept(std::is_nothrow_constructible_v<Key, Args&&...>);

        rb_tree_node_base* get_sibling() const;
        void change_color();
        bool is_left() const;
    };
}

#include "rb_tree_node.tpp"
