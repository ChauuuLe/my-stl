#pragma once

namespace mystd {
    /*Iterator*/
    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator set<Key, Compare, Allocator>::begin() {
        return iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::begin() const noexcept {
        return const_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::cbegin() const noexcept {
        return const_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator set<Key, Compare, Allocator>::end() {
        return iterator(this->tree.find_max());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::end() const noexcept {
        return const_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::cend() const noexcept {
        return const_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::reverse_iterator set<Key, Compare, Allocator>::rbegin() {
        return reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_reverse_iterator set<Key, Compare, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::reverse_iterator set<Key, Compare, Allocator>::rend() {
        return reverse_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_reverse_iterator set<Key, Compare, Allocator>::rend() const noexcept {
        return const_reverse_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator set<Key, Compare, Allocator>::crend() const noexcept {
        return const_reverse_iterator(this->tree.find_max());
    }

    /*Capacity*/
    template<
        class Key,
        class Compare,
        class Allocator
    > bool set<Key, Compare, Allocator>::empty() const noexcept{
        return this->tree.node_count == 0;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::size_type set<Key, Compare, Allocator>::size() const noexcept{
        return this->tree.size();
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::size_type set<Key, Compare, Allocator>::max_size() const noexcept{
        return this->tree.max_size();
    }

    /*Modifier*/
    template<
        class Key,
        class Compare,
        class Allocator
    > void set<Key, Compare, Allocator>::clear() noexcept {
        this->tree.clear();
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > std::pair<typename set<Key, Compare, Allocator>::iterator, bool>
            set<Key, Compare, Allocator>::insert(const value_type& value) {
        std::pair<base_type*, bool> tmp = this->tree.insert(value);

        return std::pair<node_type, bool> (*static_cast<node_type*>(tmp.first), tmp.second);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > std::pair<typename set<Key, Compare, Allocator>::iterator, bool>
            set<Key, Compare, Allocator>::insert(value_type&& value) {
        std::pair<base_type*, bool> tmp = this->tree.insert(std::move(value));

        return std::pair<node_type, bool> (*static_cast<node_type*>(tmp.first), tmp.second);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > template<class InputIt, class = enable_if_t<!is_integral<InputIt>>> 
    void set<Key, Compare, Allocator>::insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            this->tree.insert(*first);
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void set<Key, Compare, Allocator>::insert(std::initializer_list<value_type> ilist) {
        this->insert(ilist.begin(), ilist.end());
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::insert_return_type
        set<Key, Compare, Allocator>::insert(node_type&& node) {
        std::pair<base_type*, bool> tmp = this->tree.insert(std::move(node));

        return insert_return_type(
            iterator(static_cast<node_type*>(tmp.first)),
            tmp.second,
            *static_cast<node_type*>(tmp.first)
        );
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > template<class... Args>
    std::pair<typename set<Key, Compare, Allocator>::iterator, bool>
        set<Key, Compare, Allocator>::emplace(Args&&... args) {
        std::pair<base_type*, bool> tmp = this->tree.insert(std::forward<Args>(args)...);

        return std::pair<node_type, bool> (*static_cast<node_type*>(tmp.first), tmp.second);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator
        set<Key, Compare, Allocator>::erase(iterator pos) {
        return iterator(this->tree.erase(pos.ptr));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator
        set<Key, Compare, Allocator>::erase(const_iterator pos) {
        return const_iterator(this->tree.erase(pos.ptr));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::size_type
        set<Key, Compare, Allocator>::erase(const Key& key) {
        return this->tree.erase(key);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator
        set<Key, Compare, Allocator>::erase(const_iterator first, const_iterator last) {
        base_node_type* res;

        for (; first != last; ++first) {
            res = this->tree.erase(first);
        }

        return const_iterator(this->tree.erase(res));
    }

    /*Look up*/
    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::size_type
        set<Key, Compare, Allocator>::count(const Key& key) const {
        if (this->tree.find(key)) {
            return 1;
        }
        return 0;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator
        set<Key, Compare, Allocator>::find(const Key& key) {
        return iterator(this->tree.find(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator
        set<Key, Compare, Allocator>::find(const Key& key) const {
        return const_iterator(this->tree.find(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator
        set<Key, Compare, Allocator>::lower_bound(const Key& key) {
        return iterator(this->tree.lower_bound(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator
        set<Key, Compare, Allocator>::lower_bound(const Key& key) const {
        return const_iterator(this->tree.lower_bound(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::iterator
        set<Key, Compare, Allocator>::upper_bound(const Key& key) {
        return iterator(this->tree.upper_bound(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::const_iterator
        set<Key, Compare, Allocator>::upper_bound(const Key& key) const {
        return const_iterator(this->tree.upper_bound(key));
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::key_compare
        set<Key, Compare, Allocator>::key_comp() const {
        return this->compare;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > typename set<Key, Compare, Allocator>::allocator_type
        set<Key, Compare, Allocator>::get_allocator() const noexcept {
        return this->tree.get_allocator();
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > void set<Key, Compare, Allocator>::swap(set& other) 
        noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value) {
        this->tree.swap(other.tree);
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > template<class C2>
    void merge(set<Key, C2, Allocator>& source) {
        for (iterator it = source.begin(); it != source.end(); ++it) {
            if (this->tree.insert_node(it.ptr)) {
                source.tree.remove_rebalancing(it.ptr);
            }
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > template<class C2>
    void merge(set<Key, C2, Allocator>& source) {
        for (iterator it = source.begin(); it != source.end(); ++it) {
            if (this->tree.insert_node(it.ptr)) {
                source.tree.remove_rebalancing(it.ptr);
            }
        }
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > set<Key, Compare, Allocator>&
        set<Key, Compare, Allocator>::operator=(const set& oth) {
        this->tree = oth.tree;
        return *this;
    }

    template<
        class Key,
        class Compare,
        class Allocator
    > set<Key, Compare, Allocator>&
        set<Key, Compare, Allocator>::operator=(set&& oth) 
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                && std::is_nothrow_move_assignable<Compare>::value) {
        this->tree = std::move(oth.tree);
        return *this;
    }

    /*Ctor dtor*/
    template<
        class Key,
        class Compare,
        class Allocator
    > set(const Compare& comp, const Allocator& alloc)
        : tree(comp, alloc) {}
    
    template<
        class Key,
        class Compare,
        class Allocator
    > set(const Allocator& alloc)
        : tree(alloc) {}
    
    template<
        class Key,
        class Compare,
        class Allocator
    > set(const set& other)
        : tree(other) {}
    
    template<
        class Key,
        class Compare,
        class Allocator
    > set(set&& other)
        : tree(std::move(other)) {}

    template< class Key, class Compare, class Alloc >
    bool operator==(const std::set<Key, Compare, Alloc>& lhs,
                 const std::set<Key, Compare, Alloc>& rhs) {
        return equal(lhs.begin(), lhs.end(), rhs.begin());
    }
}