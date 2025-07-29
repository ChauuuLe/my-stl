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
        bool color;
        rb_tree_node_base *left = nullptr;
        rb_tree_node_base *right = nullptr;
        rb_tree_node_base *parent = nullptr;
    };

    template<class Key>
    struct rb_tree_node: public rb_tree_node_base {
        Key value;

        rb_tree_node();

        rb_tree_node(const Key& value, bool color = RED, rb_tree_node_base *parent);
        rb_tree_node(Key&& value, bool color = RED, rb_tree_node_base *parent)
            noexcept(std::is_nothrow_move_constructible_v<Key>);

        rb_tree_node_base* get_sibling() const;
        void change_color();
        bool is_left() const;
    };
}

#include "rb_tree_node.tpp"
