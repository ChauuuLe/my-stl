#pragma once
#include "rb_tree.hpp"
namespace mystd {
    /*Ctor*/
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree()
        : allocator(Allocator()), compare(Compare()), min_node(nullptr), max_node(nullptr) {
        this->header.color = RED;
        this->header.parent = nullptr;
        this->header.left = &this->header;
        this->header.right = &this->header;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const key_compare& comp, const allocator_type& alloc)
        : allocator(alloc), compare(comp), min_node(nullptr), max_node(nullptr) {
        this->header.color = RED;
        this->header.parent = nullptr;
        this->header.left = &this->header;
        this->header.right = &this->header;
    }
    
    template<
        class Key,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const allocator_type& alloc)
        : allocator(alloc), compare(Compare()), min_node(nullptr), max_node(nullptr) {
        this->header.color = RED;
        this->header.parent = nullptr;
        this->header.left = &this->header;
        this->header.right = &this->header;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::init_nil(base_node_type& nil) {
        nil.color = BLACK;
        nil.left = nil.right = &(nil);
        nil.parent = nullptr;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    >  template<class... Args>
    typename rb_tree<Key, Compare, Allocator>::base_node_type* 
        rb_tree<Key, Compare, Allocator>::make_node(bool isLeft, bool color,
                base_node_type *parent, Args&&... value_args) {
        base_node_type* node = nullptr;
        try {
            node = std::allocator_traits<Allocator>::allocate(*this, 1);
            std::allocator_traits<Allocator>::construct(*this, std::forward<Args>(value_args));
        } catch(...) {
            if (node) {
                std::allocator_traits<Allocator>::deallocate(*this, node, 1);
            }
            throw;
        }

        node->parent = parent;
        node->color = color;

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
    > bool rb_tree<Key, Compare, Allocator>::is_black(base_node_type *node) {
        return (node == nullptr || node->color == BLACK);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::flip_color(base_node_type *node) {
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
    > void rb_tree<Key, Compare, Allocator>::rotate_left(base_node_type *parent) {
        base_node_type *right_child = parent->right;
        base_node_type *left_grandchild = right_child->left;

        right_child->parent = parent->parent;
        parent->parent = right_child;
        
        right_child->left = parent;
        parent->right = left_grandchild;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::rotate_right(base_node_type *parent) {
        base_node_type *left_child = parent->left;

        base_node_type *right_grandchild = left_child->right;
        left_child->parent = parent->parent;
        parent->parent = left_child;

        left_child->right = parent;
        parent->left = right_grandchild;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::fix_up_insert(base_node_type *node, bool is_left) {
        base_node_type *cur_parent = node->parent;

        if (cur_parent->color == BLACK) {
            return;
        }

        base_node_type* cur_uncle = cur_parent->get_sibling();

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
    > template<class... Args> 
    std::pair<base_node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(Args&&... args) {
        if (!this->header.parent) {
            this->header.parent = this->make_node(false, BLACK, nullptr, std::forward<Args>(args)...);
            this->header.left = this->header.right = this->header.parent;
            this->header.parent->parent = &this->header;
 
            return std::pair<base_node_type*, bool> (this->header.parent, true);
        }

        base_node_type *traverse = this->header.parent;
        Key value = Key(std::forward<Args>(args)...);
        base_node_type *cur_parent;
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
                return std::pair<base_node_type*, bool>(traverse, false);
            }
        }

        if noexcept(std::is_move_constructible_v<Key>) {
            traverse = this->make_node(is_left, RED, cur_parent, std::move(value));
        } else {
            traverse = this->make_node(is_left, RED, cur_parent, value);
        }
        
        fix_up_insert(traverse, is_left);

        // Update min max of the tree
        if (traverse->parent == this->header.left && is_left) {
            this->header.left = traverse;
        }

        if (traverse->parent == this->header.right && !is_left) {
            this->header.right = traverse;
        }

        return std::pair<base_node_type*, bool>(traverse, false);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::free_node(base_node_type *node) {
        std::allocator_traits<Allocator>::destroy(*this, node);
        std::allocator_traits<Allocator>::deallocate(*this, node, 1);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::find(const key_type& value) const {
        base_node_type *traverse = this->header.parent;

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
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::find_min(base_node_type *node) const {
        base_node_type *traverse = node;

        while (traverse) {
            traverse = traverse->left;
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::find_max(base_node_type *node) const {
        base_node_type *traverse = node;

        while (traverse) {
            traverse = traverse->right;
        }

        return traverse;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::find_min() const {
        return this->header.left;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::find_max() const {
        return this->header.right;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::lower_bound(const key_type& value) const {
        base_node_type *traverse = this->header.parent;

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
    > typename rb_tree<Key, Compare, Allocator>::base_node_type* rb_tree<Key, Compare, Allocator>::upper_bound(const key_type& value) const {
        base_node_type *traverse = this->header.parent;

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
    > void rb_tree<Key, Compare, Allocator>::transplant(base_node_type *u, base_node_type *v) {
        if (this->header.parent == u) {
            this->header.parent = v;
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
    > void rb_tree<Key, Compare, Allocator>::fix_up_delete(base_node_type *node) {
        while (node != this->header.parent && node->color == BLACK) {
            if (node.is_left()) {
                base_node_type *sibling = node->parent->right;

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

                    node = this->header.parent;
                }
            } else {
                base_node_type *sibling = node->parent->left;

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

                    node = this->header.parent;
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
        base_node_type *need_erase = this->find(value);
        if (!need_erase) {
            return 0;
        }

        // Update min max of the tree
        if (need_erase == this->header.left) {
            if (need_erase->right) {
                this->header.left = need_erase->right;
            } else {
                this->header.left = need_erase->parent;
            }
        }

        if (need_erase == this->header.right) {
            if (need_erase->left) {
                this->header.right = need_erase->left;
            } else {
                this->header.right = need_erase->parent;
            }
        }

        base_node_type *replaced_node = need_erase;
        base_node_type *cur_replaced_pos;
        base_node_type nil;
        init_nil(nil);

        bool org_color = need_erase->color;

        if (need_erase->left == nullptr && need_erase->right == nullptr) {
            cur_replaced_pos = &(nil);
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
                replaced_node->right = &(nil);
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
