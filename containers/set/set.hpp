#pragma once

#include "../rb_tree/rb_tree.hpp"
#include "../rb_tree/rb_tree_node.hpp"
#include "../rb_tree/rb_tree_iterator.hpp"
#include "../../ultilities/functional.hpp"
#include "../../iterators/iterator_traits.hpp"
#include "../../iterators/reverse_iterator.hpp"
#include "../../ultilities/equal.hpp"
#include<cstddef>

namespace mystd {
    template<
        class Key,
        class Compare = mystd::less<Key>,
        class Allocator = std::allocator<Key>
    > class set {
    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = std::allocator_traits<Allocator>::pointer;
        using const_pointer = std::allocator_traits<Allocator>::const_pointer;

        using iterator = rb_tree_iterator<Key>;
        using const_iterator = rb_tree_iterator<const Key>;
        using reverse_iterator = mystd::reverse_iterator<iterator>;
        using const_reverse_iterator = mystd::reverse_iterator<const_iterator>;

        using node_type = rb_tree_node<Key>;
        using base_type = rb_tree_node_base;

        struct return_node {
            iterator position;
            bool inserted;
            node_type node;

            return_node(iterator pos, bool inserted, node_type node) :
                position(pos), inserted(inserted), node(node) {}
        };

        using insert_return_type = return_node;
    
    private:
        struct Identity {
            key_type operator()(const value_type& val) const noexcept {
                return val;
            }
        };

        rb_tree<key_type, value_type, Identity, value_compare, Allocator> tree;

    public:
        /*Iterator*/
        iterator begin();
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end();
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        reverse_iterator rend();
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /*Capacity*/
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;
        
        /*Modifier*/
        void clear() noexcept;

        std::pair<iterator, bool> insert(const value_type& value);
        std::pair<iterator, bool> insert(value_type&& value);
        template<class InputIt, class = enable_if_t<!is_integral<InputIt>>>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<value_type> ilist);
        insert_return_type insert(node_type&& nh);

        template<class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        iterator erase(iterator pos);
        iterator erase(const_iterator pos);
        size_type erase(const Key& key);
        iterator erase(const_iterator first, const_iterator last);

        void swap(set& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value);
        
        template<class C2>
        void merge(std::set<Key, C2, Allocator>& source);
        template<class C2>
        void merge(std::set<Key, C2, Allocator>&& source);

        /*Look up*/
        size_type count(const Key& key) const;

        iterator find(const Key& key);
        const_iterator find(const Key& key) const;

        iterator lower_bound(const Key& key);
        const_iterator lower_bound(const Key& key) const;

        iterator upper_bound(const Key& key);
        const_iterator upper_bound(const Key& key) const;

        key_compare key_comp() const;

        /*Member functions*/
        allocator_type get_allocator() const;

        set& operator=(const set& other);
        set& operator=(set&& other)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value);

        /*Ctor and dtor*/
        explicit set(const Compare& comp);
        set() : set(Compare()) {}
        explicit set(const Compare& comp,
              const Allocator& alloc = Allocator());
        explicit set(const Allocator& alloc);
        set(const set& other);
        set(set&& other);

        ~set() = default;
    };
}

#include "set.tpp"
