#pragma once

namespace mystd {
    /*Nodes for tree*/
    template<class Key>
    rb_tree_node<Key>::rb_tree_node() : value() {}

    template<class Key>
    template<class... Args>
    rb_tree_node<Key>::rb_tree_node(Args&&... value_args)
            noexcept(std::is_nothrow_constructible_v<Key, Args&&...>)
        : value(std::forward<Args>(value_args)...) {}

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
