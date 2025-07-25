#pragma once
#include "rb_tree.hpp"
namespace mystd {
    /*Ctor*/
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree()
        : root(nullptr), allocator(Allocator()), compare(Compare()) {}

    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const key_compare& comp, const allocator_type& alloc)
        : root(nullptr), allocator(alloc), compare(comp) {}
    
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const allocator_type& alloc)
        : root(nullptr), allocator(alloc), compare(Compare()) {}

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::make_node(node_type *node, const key_type& value, 
        bool is_left, bool color, node_type *parent) {
        try {
            node = std::allocator_traits<Allocator>::allocate(this->allocator, 1);
            std::allocator_traits<Allocator>::construct(this->allocator, node, value, color, parent);
        } catch(...) {
            if (node) {
                std::allocator_traits<Allocator>::deallocate(this->Allocator, node, 1);
            }
            throw;
        }

        if (!parent) {
            return;
        }

        if (is_left) {
            parent->left = node;
        } else {
            parent->right = node;
        }
    }
    
    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::rotate_node_color(node_type *node) {
        while (1) {
            if (node->left) {
                node->left.color = BLACK;
            } 
            if (node->right) {
                node->right.color = BLACK;
            }

            node->color = RED;
            if (node->parent && node->parent.color == RED) {
                node = node->parent;
            } else {
                break;
            }
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::rotate_left(node_type *parent, node_type *right_child) {
        right_child->parent = parent->parent;
        parent->parent = right_child;
        right_child->left = parent;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::rotate_right(node_type *parent, node_type *left_child) {
        left_child->parent = parent->parent;
        parent->parent = left_child;
        left_child->right = parent;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::fix_insert(node_type *node, bool is_left) {
        node_type *cur_parent = node->parent;

        if (cur_parent.color == BLACK) {
            return;
        }

        node_type* cur_uncle = cur_parent->get_sibling();

        if (!cur_uncle || cur_uncle->color == RED) {
            rotate_color_node(cur_parent);
        } else {
            if (!is_left) {
                rotate_left(cur_parent, traverse);
            }
            if (cur_parent->parent.left == cur_parent) {
                rotate_right(cur_parent->parent, cur_parent);
                cur_parent.change_color();
                cur_parent->right.change_color();
            } else {
                rotate_left(cur_parent->parent, cur_parent);
                cur_parent.change_color();
                cur_parent->left.change_color();   
            }
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > std::pair<node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(const key_type& value) {
        if (root == nullptr) {
            try {
                this->make_node(root, value, false, BLACK);
            } catch(...) {
                throw;
            }
 
            return std::pair<node_type*, bool> (root, true);
        }

        node_type *traverse = root;
        node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;
            if (value == traverse->value) {
                return std::pair<node_type*, bool>(traverse, false);
            }

            if (compare(value, traverse->value)) {
                is_left = true;
                traverse = traverse->left;
            } else {
                is_left = false;
                traverse = trarverse->right;
            }
        }

        try {
            this->make_node(traverse, value, is_left, RED, cur_parent);
        } catch(...) {
            throw;
        }
        
        fix_tree(node, is_left);

        return std::pair<node_type*, bool>(traverse, false);
    }
};