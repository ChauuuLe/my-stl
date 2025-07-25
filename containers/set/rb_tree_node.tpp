#pragma once

namespace mystd {
    /*Nodes for tree*/
    template<class Key>
    rb_tree_node<Key>::rb_tree_node() : left(nullptr), right(nullptr), color(RED), value() {}

    template<class Key>
    rb_tree_node<Key>::rb_tree_node(const Key& value, bool color, rb_tree_node *parent = nullptr)
        : parent(parent), color(color), value(value) {};

    template<class Key>
    rb_tree_node<Key>::rb_tree_node(Key&& value, bool color, rb_tree_node *parent = nullptr) 
        noexcept(std::is_nothrow_move_constructible_v<Key>)
        : parent(parent), color(color), value(std::move(value)) {};

    template<class Key>
    rb_tree_node<Key>& rb_tree_node<Key>::operator=(const rb_tree_node& other) {
        this->left = other.left;
        this->right = other.right;
        this->value = other.value;
        this->color = other.color;

        return *this;
    }

    template<class Key>
    rb_tree_node<Key>& rb_tree_node<Key>::operator=(rb_tree_node&& other)
            noexcept(std::is_nothrow_move_assignable_v<Key>) {
        this->left = other.left;
        this->right = other.right;
        this->value = std::move(other.value);
        this->color = other.color;
        
        other.left = nullptr;
        other.right = nullptr;
    
        return *this;
    }

    template<class Key>
    rb_tree_node<Key>::~rb_tree_node() = default;

    template<class Key>
    rb_tree_node<Key>* rb_tree_node<Key>::get_sibling() const {
        if (node->parent.left == this) {
            return node->parent.right;
        }

        return node->parent.left;
    }
    template<class Key>
    void rb_tree_node<Key>::change_color() {
        this->color = !this->color;
    }
};
