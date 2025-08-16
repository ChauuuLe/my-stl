#pragma once

#include "../rb_tree/rb_tree.hpp"
#include "../rb_tree/rb_tree_node.hpp"
#include "../rb_tree/rb_tree_iterator.hpp"
#include "../../ultilities/functional.hpp"
#include "../../iterators/iterator_traits.hpp"
#include "../../iterators/reverse_iterator.hpp"
#include "../../ultilities/equal.hpp"
#include <functional>
#include <cstddef>

namespace mystd {
    template<
        class Key,
        class T,
        class Compare = mystd::less<Key>,
        class Allocator = std::allocator<std::pair<const Key, T>> 
    > class map {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = std::allocator_traits<Allocator>::pointer;
        using const_pointer = std::allocator_traits<Allocator>::const_pointer;
        
        using iterator = rb_tree_iterator<value_type>;
        using const_iterator = rb_tree_iterator<const value_type>;
        using reverse_iterator = mystd::reverser_iterator<iterator>;
        using const_reverser_iterator = mystd::const_reverse_iterator<const_iterator>;

        using base_type = rb_tree_node_base;
        using node_type = rb_tree_node<value_type>;

        class value_compare: std::binary_function<value_type, value_type, bool> {
            friend class map;
        protected:
            Compare comp;
            value_compare(Compare c): comp(c) {}
        public:
            bool operator()(const value_type& arg1, const value_type& arg2) const {
                return comp(arg1.first, arg1.second);
            }
        };

        struct return_node {
            iterator position;
            bool inserted;
            node_type node;

            return_node(iterator pos, bool inserted, node_type node) :
                position(pos), inserted(inserted), node(node) {}
        };

        using insert_return_type = return_node;
    private:
        struct SelectFirst {
            key_type operator()(const value_type& val) const noexcept {
                return val.first;
            }
        };

        rb_tree<key_type, value_type, SelectFirst, value_compare, Allocator> tree;
    public:
        /*Element access*/
        mapped_type& at(const Key& key);
        const mapped_type& at(const Key& key) const;

        mapped_type& operator[](const Key& key);
        mapped_type& operator[](Key&& key);

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
        template<class P>
        std::pair<iterator, bool> insert(P&& value);
        std::pair<iterator, bool> insert(value_type&& value);
        template<class InputIt, class = enable_if_t<!is_integral<InputIt>>>
        void insert(InputIt first, InputIt last);
        void insert(std::initializer_list<value_type> ilist);
        insert_return_type insert(node_type&& nh);

        template<class M>
        std::pair<iterator, bool> insert_or_assign(const Key& key, M&& obj);
        template<class M>
        std::pair<iterator, bool> insert_or_assign(Key&& key, M&& obj);

        template<class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);

        template<class... Args>
        std::pair<iterator, bool> try_emplace(const Key& k, Args&&... args);
        template<class... Args>
        std::pair<iterator, bool> try_emplace(Key&& k, Args&&... args);

        iterator erase(iterator pos);
        iterator erase(const_iterator pos);
        size_type erase(const Key& key);
        void swap(map& other)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value || 
                    || std::is_nothrow_move_assignable<Compare>::value);

        node_type extract(const_iterator pos);
        node_type extract( const Key& k );

        template<class C2>
        void merge(std::map<Key, T, C2, Allocator>& source);
        template<class C2>
        void merge(std::map<Key, T, C2, Allocator>&& source);
        
        /*Look up*/
        size_type count(const Key& key) const;
        iterator find(const Key& key);
        const_iterator find(const Key& key) const;
        bool contains(const Key& key) const;
        std::pair<iterator, iterator> equal_range(const Key& key);
        std::pair<const_iterator, const_iterator>
            equal_range(const Key& key) const;

        iterator lower_bound( const Key& key );
        const_iterator lower_bound( const Key& key ) const;

        iterator upper_bound( const Key& key );
        const_iterator upper_bound( const Key& key ) const;

        key_compare key_comp() const;
        value_compare value_comp() const;
        
        /*Ctor and dtor*/
        map() : map(Compare()) {}
        explicit map( const Compare& comp,
                    const Allocator& alloc = Allocator() );
        explicit map( const Allocator& alloc );
        map( const map& other );
        map( map&& other );
        
        map& operator=( const map& other );
        map& operator=( map&& other );

        ~map() = default;
    }
}

#include "map.tpp"
