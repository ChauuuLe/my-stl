#pragma once

#include "rb_tree_node.hpp"
#include "../../memory/uninitialized_memory.hpp"
#include "../../ultilities/functional.hpp"
#include <utility>
#include <memory>
#include <type_traits>

namespace mystd {
    template<class Allocator, class Key>
    using node_alloc = std::allocator_traits<Allocator>::template rebind_alloc<rb_tree_node<Key>>;

    template<
        class Key,
        class Compare = mystd::less<Key>,
        class Allocator = std::allocator<Key>
    > struct rb_tree : private node_alloc<Allocator, Key> {
        using key_type = Key;
        using allocator_type = Allocator;
        using key_compare = Compare;
        using node_type = typename rb_tree_node<Key>;
        using base_node_type = rb_tree_node_base;
        using size_type = size_t;
        using node_allocator = node_allocator<Allocator, Key>;

        /*
        Root is header parent
        Right is max node
        Left is min node
        */
        base_node_type header;
        key_compare compare;
        size_type node_count;

        rb_tree();
        explicit rb_tree(const key_compare& comp, const node_allocator& alloc = node_allocator());
        explicit rb_tree(const node_allocator& alloc);
        rb_tree(const rb_tree& rhs);
        rb_tree(rb_tree&& rhs);

        rb_tree& operator=(const rb_tree& rhs);
        rb_tree& operator=(rb_tree&& rhs);

        allocator_type get_allocator() const;
        const node_allocator& get_node_allocator() const;
        node_allocator& get_node_allocator();

        void copy_tree(base_node_type** this_node, base_node_type* rhs_node);
        void delete_subtree(base_node_type* node);
        
        template<class... Args>
        std::pair<base_node_type*, bool> insert(Args&&... args);
        size_type erase(const key_type& value);
        base_node_type* find(const key_type& value) const;

        base_node_type* find_max(base_node_type *node) const;
        base_node_type* find_min(base_node_type *node) const;
        base_node_type* find_max() const;
        base_node_type* find_min() const;
        base_node_type* lower_bound(const key_type& value) const;
        base_node_type* upper_bound(const key_type& value) const;

        template<class... Args>
        base_node_type* make_node(bool is_left, bool color, base_node_type *parent, Args&&... args);
        void free_node(base_node_type *node) noexcept;

        void init_nil(base_node_type& nil);
        void init_header();

        const base_node_type* base(const node_type* node) const;
        const node_type* noep(const base_node_type* node) const;
        base_node_type* base(node_type* node) const;
        node_type* noep(base_node_type* node) const;

        bool is_black(base_node_type *node);
        void flip_color(base_node_type *node);
        void rotate_left(base_node_type *parent);
        void rotate_right(base_node_type *parent);
        void transplant(base_node_type *u, base_node_type* v);

        void fix_up_insert(base_node_type *node, bool is_left);
        void fix_up_delete(base_node_type *node);
    };
}

#include "rb_tree.tpp"
