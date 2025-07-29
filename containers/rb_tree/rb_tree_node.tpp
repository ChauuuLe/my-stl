#pragma once

namespace mystd {
    /*Nodes for tree*/
    template<class Key>
    rb_tree_node<Key>::rb_tree_node() : color(BLACK), value() {}

    template<class Key>
    rb_tree_node<Key>::rb_tree_node(const Key& value, bool color, rb_tree_node_base *parent)
        : parent(parent), color(color), value(value) {}

    template<class Key>
    rb_tree_node<Key>::rb_tree_node(Key&& value, bool color, rb_tree_node_base *parent) 
        noexcept(std::is_nothrow_move_constructible_v<Key>)
        : parent(parent), color(color), value(std::move(value)) {}

    template<class Key>
    rb_tree_node_base* rb_tree_node<Key>::get_sibling() const {
        if (node->parent->left == this) {
            return node->parent->right;
        }

        return node->parent->left;
    }

    template<class Key>
    void rb_tree_node<Key>::change_color() {
        this->color = !this->color;
    }

    template<class Key>
    bool rb_tree_node<Key>::is_left() const {
        return (this->parent && this->parent->left == this);
    }
};
