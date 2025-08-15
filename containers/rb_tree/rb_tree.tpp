#pragma once
#include "rb_tree.hpp"
namespace mystd {
    /*Ctor*/
    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree()
        : node_allocator(node_allocator()), compare(Compare()), node_count(0) {
        init_header();
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const key_compare& comp, const node_allocator& alloc)
        : node_allocator(alloc), compare(comp), min_node(nullptr), node_count(0) {
        init_header();
    }
    
    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const node_allocator& alloc)
        : node_allocator(alloc), compare(Compare()), node_count(0) {
        init_header();
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(const rb_tree& rhs)
        : node_allocator(std::allocator_traits<node_allocator>::select_on_container_copy_construction(rhs.get_node_allocator())),
          compare(rhs.compare), node_count(rhs.node_count) {
        init_header();

        try {
            copy_tree(&(this->header.parent), rhs.header.parent);

            if (this->header.parent) {
                this->header.right = this->find_max(this->header.parent);
                this->header.left = this->find_min(this->header.parent);    
            }
        } catch (...) {
            delete_subtree(this->header.parent);
            throw;
        }
    }
    
    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>::rb_tree(rb_tree&& rhs)
        : node_allocator(std::move(rhs.get_allocator())),
            compare(std::move(rhs.compare)), node_count(rhs.node_count) {
        init_header();
        if (rhs.header.left != rhs.header) {
            this->header.left = rhs.header.left;
        }
        if (rhs.header.right != rhs.header) {
            this->header.right = rhs.header.right;
        }
        this->header.parent = rhs.header.parent;

        rhs.init_header();
        rhs.node_count = 0;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>& rb_tree<Key, Compare, Allocator>::operator=(const rb_tree& rhs) {
        if constexpr(std::allocator_traits<node_allocator>::propagate_on_container_copy_assignment::value) {
            node_allocator old_alloc = *this;
            node_allocator& this_allocator = this->get_node_allocator();

            init_header();
            this_allocator = rhs.get_node_allocator();

            try {
                copy_tree(&(this->header.parent), rhs.header.parent);
            } catch(...) {
                delete_subtree(this->header.parent);
                this_allocator = old_alloc;
                this->header.parent = old_root;
                throw;
            }

            this_allocator = old_alloc;
            delete_subtree(old_root);
        } else {
            base_node_type* old_root = this->header.parent;
            init_header();

            try {
                copy_tree(&(this->header.parent), rhs.header.parent);
            } catch(...) {
                delete_subtree(this->header.parent);
                this->header.parent = old_root;
                throw;
            }

            delete_subtree(old_root);
        }

        this->compare = rhs.compare;
        this->node_count = rhs.node_count;
        if (this->header.parent) {
            this->header.right = this->find_max(this->header.parent);
            this->header.left = this->find_min(this->header.parent);    
        }
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > rb_tree<Key, Compare, Allocator>& rb_tree<Key, Compare, Allocator>::operator=(rb_tree&& rhs)
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value) {
        if constexpr(std::allocator_traits<node_allocator>::propagate_on_container_move_assignment::value) {
            node_allocator& this_allocator = this->get_node_allocator();
            this_allocator = rhs.get_node_allocator();
        }

        this->node_count = rhs.node_count;
        this->compare = std::move(rhs.compare);
        this->header.parent = rhs.header.parent;
        
        init_header();
        if (rhs.header.left != rhs.header) {
            this->header.left = rhs.header.left;
        }
        if (rhs.header.right != rhs.header) {
            this->header.right = rhs.header.right;
        }
        
        rhs.init_header();
        rhs.node_count = 0;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::swap(rb_tree& other)
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value) {
        std::swap(this->size, other.size);
        std::swap(compare, other.compare);
        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_swap::value) {
            std::swap(get_node_allocator(), other.get_node_allocator());
        }

        if (this->header.left == &(this->header)) {
            this->header.left = &(other.header);
            this->header.right = &(other.header);
        }

        if (other.header.right == &(other.header)) {
            other.header.left = &(this->header);
            other.header.right = &(this->header);
        }

        std::swap(this->header, other.header);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::allocator_type
            rb_tree<Key, Compare, Allocator>::get_allocator() const {
        return allocator_type(this->get_node_allocator());
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::node_allocator&
            rb_tree<Key, Compare, Allocator>::get_node_allocator() {
        return *static_cast<node_allocator*>(this);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > const typename rb_tree<Key, Compare, Allocator>::node_allocator&
            rb_tree<Key, Compare, Allocator>::get_node_allocator() const {
        return *static_cast<const node_allocator*>(this);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void copy_tree(base_node_type** this_node, base_node_type* rhs_node) {
        base_node_type* par = &(this->header);

        while ((*this_node) != &(this->header)) {
            if (!rhs_node) {
                this_node = &par;
                rhs_node = rhs_node->parent;
                continue;
            }

            if (!(*this_node)) {
                *this_node = this->make_node(rhs_node->is_left(), rhs_node->color, par, noep(rhs_node)->value);
            }

            if (rhs_node->left && (*this_node)->left == nullptr) {
                par = *this_node;
                rhs_node = rhs_node->left;
                this_node = &(*this_node)->left;
                continue;
            }

            if (rhs_node->right && (*this_node)->right == nullptr) {
                par = *this_node;
                rhs_node = rhs_node->right;
                this_node = &(*this_node)->right;
                continue;
            }

            this_node = &par;
            rhs_node = rhs_node->parent;
        }
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void delete_subtree(base_node_type* node) noexcept {
        if (!node) {
            return;
        }

        base_node_type *par = &(this->header);

        while (node != &(this->header)) {
            if (node->left) {
                node = node->left;
                continue;
            }

            if (node->right) {
                node = node->right;
                continue;
            }

            this->free_node(node);
            node = par;
        }
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::init_nil(base_node_type& nil) {
        nil.color = BLACK;
        nil.left = nil.right = &(nil);
        nil.parent = nullptr;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::init_header() {
        this->header.color = RED;
        this->header.parent = nullptr;
        this->header.left = &this->header;
        this->header.right = &this->header;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    >  template<class... Args>
    typename rb_tree<Key, Compare, Allocator>::base_node_type* 
        rb_tree<Key, Compare, Allocator>::make_node(bool is_left, bool color,
                base_node_type *parent, Args&&... value_args) {
        base_node_type* node = nullptr;
        try {
            node = std::allocator_traits<node_allocator>::allocate(*this, 1);
            std::allocator_traits<node_allocator>::construct(*this, node, std::forward<Args>(value_args));
        } catch(...) {
            if (node) {
                std::allocator_traits<node_allocator>::deallocate(*this, node, 1);
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
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::free_node(base_node_type *node) noexcept {
        std::allocator_traits<node_allocator>::destroy(*this, node);
        std::allocator_traits<node_allocator>::deallocate(*this, node, 1);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    >  const typename rb_tree<Key, Compare, Allocator>::base_node_type*
        rb_tree<Key, Compare, Allocator>::base(const node_type* node) const noexcept {
        return static_cast<base_node_type*>(node);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    >  typename rb_tree<Key, Compare, Allocator>::base_node_type*
        rb_tree<Key, Compare, Allocator>::base(node_type* node) const noexcept {
        return static_cast<base_node_type*>(node);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    >  typename rb_tree<Key, Compare, Allocator>::node_type*
        rb_tree<Key, Compare, Allocator>::noep(const base_node_type* node) const noexcept {
        return static_cast<node_type*>(node);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > const typename rb_tree<Key, Compare, Allocator>::node_type*
        rb_tree<Key, Compare, Allocator>::noep(base_node_type* node) const noexcept {
        return static_cast<node_type*>(node);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::key_type
        rb_tree<Key, Compare, Allocator>::get_key(const value_type& val) const noexcept {
        return key_of_val()(val);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > bool rb_tree<Key, Compare, Allocator>::is_black(base_node_type *node) {
        return (node == nullptr || node->color == BLACK);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > template<class... Args> 
    std::pair<base_node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(Args&&... args) {
        if (!this->header.parent) {
            this->header.parent = this->make_node(false, BLACK, nullptr, std::forward<Args>(args)...);
            this->header.left = this->header.right = this->header.parent;
            this->header.parent->parent = &this->header;

            this->node_count++;
 
            return std::pair<base_node_type*, bool> (this->header.parent, true);
        }

        base_node_type *traverse = this->header.parent;
        Key value = Key(std::forward<Args>(args)...);
        base_node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(this->key_of_val(value), this->key_of_val(noep(traverse)->value))) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(this->key_of_val(noep(traverse)->value), this->key_of_val(value))){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return std::pair<base_node_type*, bool>(traverse, false);
            }
        }

        if constexpr (std::is_move_constructible_v<Key>) {
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

        this->node_count++;
        return std::pair<base_node_type*, bool>(traverse, true);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > template<class... Args> 
    std::pair<base_node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(const value_type& value) {
        if (!this->header.parent) {
            this->header.parent = this->make_node(false, BLACK, nullptr, value);
            this->header.left = this->header.right = this->header.parent;
            this->header.parent->parent = &this->header;

            this->node_count++;
 
            return std::pair<base_node_type*, bool> (this->header.parent, true);
        }

        base_node_type *traverse = this->header.parent;
        base_node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(this->key_of_val(value), this->key_of_value(noep(traverse)->value))) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(this->key_of_value(noep(traverse)->value), this->key_of_val(value))){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return std::pair<base_node_type*, bool>(traverse, false);
            }
        }

        
        traverse = this->make_node(is_left, RED, cur_parent, value);
        
        fix_up_insert(traverse, is_left);

        // Update min max of the tree
        if (traverse->parent == this->header.left && is_left) {
            this->header.left = traverse;
        }

        if (traverse->parent == this->header.right && !is_left) {
            this->header.right = traverse;
        }

        this->node_count++;
        return std::pair<base_node_type*, bool>(traverse, true);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > template<class... Args> 
    std::pair<base_node_type*, bool> rb_tree<Key, Compare, Allocator>::insert(value_type&& value) {
        if (!this->header.parent) {
            this->header.parent = this->make_node(false, BLACK, nullptr, std::move(value));
            this->header.left = this->header.right = this->header.parent;
            this->header.parent->parent = &this->header;

            this->node_count++;
 
            return std::pair<base_node_type*, bool> (this->header.parent, true);
        }

        base_node_type *traverse = this->header.parent;
        base_node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(this->key_of_val(value), this->key_of_val(noep(traverse)->value))) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(this->key_of_val(noep(traverse)->value), this->key_of_val(value))){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return std::pair<base_node_type*, bool>(traverse, false);
            }
        }

        
        traverse = this->make_node(is_left, RED, cur_parent, std::move(value));
        
        fix_up_insert(traverse, is_left);

        // Update min max of the tree
        if (traverse->parent == this->header.left && is_left) {
            this->header.left = traverse;
        }

        if (traverse->parent == this->header.right && !is_left) {
            this->header.right = traverse;
        }

        this->node_count++;
        return std::pair<base_node_type*, bool>(traverse, true);
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > template<class... Args> 
    typename rb_tree<Key, Compare, Allocator>::size_type
        rb_tree<Key, Compare, Allocator>::insert_node(node_type* node) {
        if (!this->header.parent) {
            this->header.parent = node;
            this->header.left = this->header.right = this->header.parent;
            this->header.parent->parent = &this->header;

            this->node_count++;
 
            return 1;
        }

        base_node_type *traverse = this->header.parent;
        base_node_type *cur_parent;
        bool is_left = false;

        while (traverse) {
            cur_parent = traverse;

            if (compare(this->key_of_val(node->value), this->key_of_val(noep(traverse)->value)) {
                is_left = true;
                traverse = traverse->left;
            } else (compare(this->key_of_val(noep(traverse)->value), this->key_of_val(node->value))){
                is_left = false;
                traverse = trarverse->right;
            } else {
                return 0;
            }
        }

        
        traverse = node;
        
        fix_up_insert(traverse, is_left);

        // Update min max of the tree
        if (traverse->parent == this->header.left && is_left) {
            this->header.left = traverse;
        }

        if (traverse->parent == this->header.right && !is_left) {
            this->header.right = traverse;
        }

        this->node_count++;
        return 1;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
            rb_tree<Key, Compare, Allocator>::find(const value_type& value) const {
        base_node_type *traverse = this->header.parent;

        while (traverse) {
            if (this->compare(this->key_of_val(value), this->key_of_val(noep(traverse)->value))) {
                traverse = traverse->left;
            } else if (this->compare(this->key_of_val(noep(traverse)->value), this->key_of_val(value))) {
                traverse = traverse->right;
            } else {
                break;
            }
        }

        return traverse;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
            rb_tree<Key, Compare, Allocator>::find_min() const noexcept {
        return this->header.left;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
            rb_tree<Key, Compare, Allocator>::find_max() const noexcept {
        return this->header.right;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
            rb_tree<Key, Compare, Allocator>::lower_bound(const value_type& value) const {
        base_node_type *traverse = this->header.parent;

        while (traverse) {
            if (this->key_of_val(this->compare(noep(traverse)->value), this->key_of_val(value))) {
                traverse = traverse->right;
            } else {
                if (!traverse->left ||
                        this->key_of_val(this->compare(noep(traverse->left)->value), this->key_of_val(value))) {
                    break;
                } else {
                    traverse = traverse->left;
                }
            }
        }

        if (!traverse) {
            return &(this->header);
        }

        return traverse;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
            rb_tree<Key, Compare, Allocator>::upper_bound(const value_type& value) const {
        base_node_type *traverse = this->header.parent;

        while (traverse) {
            if (!this->compare(this->key_of_val(value), this->key_of_val(noep(traverse)->value))) {
                traverse = traverse->right;
            } else {
                if (!traverse->left
                        || !this->compare(this->key_of_val(value), this->key_of_val(noep(traverse->left)->value))) {
                    break;
                } else {
                    traverse = traverse->left;
                }
            }
        }

        if (!traverse) {
            return &(this->header);
        }

        return traverse;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
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
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > void rb_tree<Key, Compare, Allocator>::remove_rebalancing(base_node_type *need_erase) {
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

        this->node_count--;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::size_type rb_tree<Key, Compare, Allocator>::erase(const value_type& value) {
        base_node_type *need_erase = this->find(value);
        if (!need_erase) {
            return 0;
        }

        this->remove_rebalancing(need_erase);
        
        this->free_node(need_erase);
        return 1;
    }

    template<
        class Key,
        class Value,
        class KeyOfVal,
        class Compare,
        class Allocator
    > typename rb_tree<Key, Compare, Allocator>::base_node_type*
        rb_tree<Key, Compare, Allocator>::erase(base_node_type *node) {
        base_node_type *next_node = node->next();

        this->remove_rebalancing(node);
        
        this->free_node(node);
        return next_node;
    }
};
