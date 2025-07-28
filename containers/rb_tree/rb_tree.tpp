#pragma once
#include "rb_tree.hpp"
namespace mystd {
    /*Ctor*/
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree()
        : root(nullptr), allocator(Allocator()), compare(Compare()), min_node(nullptr), max_node(nullptr) {
        init_nil();
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const key_compare& comp, const allocator_type& alloc)
        : root(nullptr), allocator(alloc), compare(comp), min_node(nullptr), max_node(nullptr) {
        init_nil();
    }
    
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const allocator_type& alloc)
        : root(nullptr), allocator(alloc), compare(Compare()), min_node(nullptr), max_node(nullptr) {
        init_nil();
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::init_nil() {
        this->nil.color = BLACK;
        this->nil.left = this->nil.right = &(this->nil);
        this->nil.parent = nullptr;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* 
        rb_tree<Key, Compare, Allocator>::make_node(const key_type& value, 
                bool isLeft, bool color, node_type *parent) {
        node_type* node = nullptr;
        try {
            node = std::allocator_traits<Allocator>::allocate(this->allocator, 1);
            std::allocator_traits<Allocator>::construct(this->allocator, node, value, color, parent);
        } catch(...) {
            if (node) {
                std::allocator_traits<Allocator>::deallocate(this->Allocator, node, 1);
            }
            throw;
        }

        if (parent) {
            if (is_left) {
                parent->left = node;
            } else {
                parent->right = node;
            }    
        }

        return node;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* 
        rb_tree<Key, Compare, Allocator>::make_node(key_type&& value, 
                bool isLeft, bool color, node_type *parent) {
        node_type* node = nullptr;
        try {
            node = std::allocator_traits<Allocator>::allocate(this->allocator, 1);
            std::allocator_traits<Allocator>::construct(this->allocator, node, std::move(value), color, parent);
        } catch(...) {
            if (node) {
                std::allocator_traits<Allocator>::deallocate(this->Allocator, node, 1);
            }
            throw;
        }

        if (parent) {
            if (is_left) {
                parent->left = node;
            } else {
                parent->right = node;
            }    
        }

        return node;
    }
    
    template<
        class Key,
        class Compare,
        class Allocator
    > bool rb_tree<Key, Compare, Allocator>::is_black(node_type *node) {
        return (node == nullptr || node->color == BLACK);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::flip_color(node_type *node) {
        while (1) {
            if (node->left) {
                node->left->color = BLACK;
            } 
            if (node->right) {
                node->right->color = BLACK;
            }

            node->color = RED;
            if (node->parent && node->parent->color == RED) {
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
    > void rb_tree<Key, Compare, Allocator>::rotate_left(node_type *parent) {
        node_type *right_child = parent->right;
        node_type *left_grandchild = right_child->left;

        right_child->parent = parent->parent;
        parent->parent = right_child;
        
        right_child->left = parent;
        parent->right = left_grandchild;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::rotate_right(node_type *parent) {
        node_type *left_child = parent->left;

        node_type *right_grandchild = left_child->right;
        left_child->parent = parent->parent;
        parent->parent = left_child;

        left_child->right = parent;
        parent->left = right_grandchild;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::fix_up_insert(node_type *node, bool is_left) {
        node_type *cur_parent = node->parent;

        if (cur_parent->color == BLACK) {
            return;
        }

        node_type* cur_uncle = cur_parent->get_sibling();

        if (cur_uncle->color == RED) {
            rotate_color_node(cur_parent);
        } else {
            if (!is_left) {
                rotate_left(cur_parent);
            }
            if (cur_parent->is_left()) {
                rotate_right(cur_parent->parent);
                cur_parent->change_color();
                cur_parent->right->change_color();
            } else {
                rotate_left(cur_parent->parent);
                cur_parent->change_color();
                cur_parent->left->change_color();   
            }
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > std::pair<node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(const key_type& value) {
        if (!root) {
            root = this->make_node(value, false, BLACK);
            this->min_node = this->max_node = root;
 
            return std::pair<node_type*, bool> (root, true);
        }

        node_type *traverse = root;
        node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(value, traverse->value)) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(traverse->value, value)){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return std::pair<node_type*, bool>(traverse, false);
            }
        }

        traverse = this->make_node(value, is_left, RED, cur_parent);
        
        fix_up_insert(traverse, is_left);

        // Update min max of the tree
        if (traverse->parent == this->min_node && is_left) {
            this->min_node = traverse;
        }

        if (traverse->parent == this->max_node && !is_left) {
            this->max_node = traverse;
        }

        return std::pair<node_type*, bool>(traverse, false);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > std::pair<node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(key_type&& value) {
        static_assert(std::is_move_constructible_v<key_type>,
            "Value should be move constructible");
   
        if (!root) {
            root = this->make_node(std::move(value), false, BLACK);
 
            return std::pair<node_type*, bool> (root, true);
        }

        node_type *traverse = root;
        node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(value, traverse->value)) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(traverse->value, value)){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return std::pair<node_type*, bool>(traverse, false);
            }
        }

        traverse = this->make_node(std::move(value), is_left, RED, cur_parent);
        
        fix_up_insert(node, is_left);

        // Update min max of the tree
        if (traverse->parent == this->min_node && is_left) {
            this->min_node = traverse;
        }

        if (traverse->parent == this->max_node && !is_left) {
            this->max_node = traverse;
        }

        return std::pair<node_type*, bool>(traverse, false);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::free_node(node_type *node) {
        std::allocator_traits<Allocator>::destroy(this->allocator, node);
        std::allocator_traits<Allocator>::deallocate(this->allocator, node, 1);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::find(const key_type& value) const {
        node_type *traverse = root;

        while (traverse) {
            if (this->compare(value, traverse->value)) {
                traverse = traverse->left;
            } else if (this->compare(traverse->value, value)) {
                traverse = traverse->right;
            } else {
                break;
            }
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::find_min(node_type *node) const {
        node_type *traverse = node;

        while (traverse) {
            traverse = traverse->left;
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::find_max(node_type *node) const {
        node_type *traverse = node;

        while (traverse) {
            traverse = traverse->right;
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::find_min() const {
        return this->min_node;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::find_max() const {
        return this->max_node;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::lower_bound(const key_type& value) const {
        node_type *traverse = root;

        while (traverse) {
            if (this->compare(traverse->value, value)) {
                traverse = traverse->right;
            } else {
                if (!traverse->left || this->compare(traverse->left->value, value)) {
                    break;
                } else {
                    traverse = traverse->left;
                }
            }
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_type* rb_tree<Key, Compare, Allocator>::upper_bound(const key_type& value) const {
        node_type *traverse = root;

        while (traverse) {
            if (!this->compare(value, traverse->value)) {
                traverse = traverse->right;
            } else {
                if (!traverse->left || !this->compare(value, traverse->left->value)) {
                    break;
                } else {
                    traverse = traverse->left;
                }
            }
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::transplant(node_type *u, node_type *v) {
        if (this->root == u) {
            this->root = v;
        } else if (u->is_left()) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }

        v->parent = v->parent;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::fix_up_delete(node_type *node) {
        while (node != this->root && node->color == BLACK) {
            if (node.is_left()) {
                node_type *sibling = node->parent->right;

                if (!is_black(sibling)) {
                    /* In this case, node is black and sibling of node is red so parent
                    p of node is black. Change p to red and sibling to black 
                    */ 
                    node->parent->color = RED;
                    sibling->color = BLACK;
                    rotate_left(node->parent);
                    sibling = node->parent->right;
                }

                if (!sibling || (is_black(sibling->right) && is_black(sibling->left))) {
                    if (sibling) {
                        sibling->color = RED;
                    }
                    node = node->parent;
                } else {
                    if (is_black(sibling->right)) {
                        sibling->color = RED;
                        sibling->left->color = BLACK;
                        rotate_right(sibling);
                        sibling = node->parent->right;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotate_left(node->parent);

                    node = this->root;
                }
            } else {
                node_type *sibling = node->parent->left;

                if (!is_black(sibling)) {
                    node->parent->color = RED;
                    sibling->color = BLACK;
                    rotate_right(node->parent);
                    sibling = node->parent->left;
                }

                if (!sibling || (is_black(sibling->right) && is_black(sibling->left))) {
                    if (sibling) {
                        sibling->color = RED;
                    }
                    node = node->parent;
                } else {
                    if (is_black(sibling->left)) {
                        sibling->color = RED;
                        sibling->right->color = BLACK;
                        rotate_left(sibling);
                        sibling = node->parent->left;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotate_right(node->parent);

                    node = this->root;
                }
            }
        }

        node->color = BLACK;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::size_type rb_tree<Key, Compare, Allocator>::erase(const key_type& value) {
        node_type *need_erase = this->find(value);
        if (!need_erase) {
            return 0;
        }

        // Update min max of the tree
        if (need_erase == this->min_node) {
            if (need_erase->right) {
                this->min_node = need_erase->right;
            } else {
                this->min_node = need_erase->parent;
            }
        }

        if (need_erase == this->max_node) {
            if (need_erase->left) {
                this->max_node = need_erase->left;
            } else {
                this->max_node = need_erase->parent;
            }
        }

        node_type *replaced_node = need_erase;
        node_type *cur_replaced_pos;

        bool org_color = need_erase->color;

        if (need_erase->left == nullptr && need_erase->right == nullptr) {
            cur_replaced_pos = &(this->nil);
            cur_replaced_pos->parent = need_erase->parent;
        } else if (need_erase->left == nullptr) {
            replaced_node = need_erase->right;
            cur_replaced_pos = replaced_node;
            this->transplant(need_erase, replaced_node);
        } else if (need_erase->right == nullptr) {
            replaced_node = need_erase->left;
            cur_replaced_pos = replaced_node;
            this->transplant(need_erase, replaced_node);
        } else {
            //Remove the replaced node from the tree to attached to the need-to-erased node position
            replaced_node = this->find_min(need_erase->right);
            org_color = replaced_node->color;

            if (replaced_node->right == nullptr) {
                replaced_node->right = &(this->nil);
            }
            cur_replaced_pos = replaced_node->right;

            if (need_erase->right == replaced_node) {
                cur_replaced_pos->parent = replaced_node->parent;
            } else {
                transplant(replaced_node, cur_replaced_pos);
            }
            replaced_node->right = need_erase->right;
            replaced_node->color = need_erase->color;
            transplant(need_erase, replaced_node);
        }

        if (org_color == BLACK) {
            fix_up_delete(cur_replaced_pos);
        }

        this->free_node(need_erase);
        return 1;
    }
};
