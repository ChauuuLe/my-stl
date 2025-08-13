#pragma once

namespace mystd {
    /*Nodes for tree*/
    template<class Key>
    rb_tree_node<Key>::rb_tree_node() : value() {}

    template<class Key>
    template<class... Args>
    rb_tree_node<Key>::rb_tree_node(Args&&... value_args)
            : value(std::forward<Args>(value_args)...) {}

    template<class Key>
    rb_tree_node<Key>::rb_tree_node(rb_tree_node&& node)
            : value(std::move(node.value)), left(node.left), right(node.right),
              parent(node.parent), color(node.color) {}

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
        return (this->parent && this->parent->parent != this && this->parent->left == this);
    }

    template<class Key>
    rb_tree_node_base* rb_tree_node<Key>::next() const {
        rb_tree_node_base* cur = this;
        rb_tree_node_base* res;
        
        if (cur->right) {
            res = cur->right;
            while (res) {
                res = res->left;
            }
        } else {
            res = cur;
            cur = cur->parent;

            while (cur == res->right) {
                res = cur;
                cur = cur->parent;
            }

            // This case not happen when res is header and the max is root
            if (res->right != cur) {
                res = cur;
            }
        }

        return res;
    }

    template<class Key>
    rb_tree_node_base* rb_tree_node<Key>::prev() const {
        rb_tree_node_base* cur = this;
        rb_tree_node_base* res;

        if (cur->color == RED && cur->parent->parent == cur) {
            return cur->right;
        }

        if (cur->left) {
            res = cur->left;
            
            while (res) {
                res = res->right;
            }
        } else {
            res = cur;
            cur = cur->parent;

            while (cur->left == res) {
                res = cur;
                cur = cur->parent;
            }

            res = cur;
        }

        return res;
    }
};
