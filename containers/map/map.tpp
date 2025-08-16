#pragma once

namespace mystd {
    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::mapped_type& 
        map<Key, T, Compare, Allocator>::at(const Key& key) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));

        if (!tmp) {
            throw std::out_of_range;
        }

        return tmp->value.second;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > const typename map<Key, T, Compare, Allocator>::mapped_type& 
        map<Key, T, Compare, Allocator>::at(const Key& key) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));

        if (!tmp) {
            throw std::out_of_range;
        }

        return tmp->value.second;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > const typename map<Key, T, Compare, Allocator>::mapped_type& 
        map<Key, T, Compare, Allocator>::operator[](const Key& key) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));
        
        if (!tmp) {
            tmp = static_cast<node_type*> (this->insert(key, mapped_type()).first);
        }

        return tmp->value.second;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > const typename map<Key, T, Compare, Allocator>::mapped_type& 
        map<Key, T, Compare, Allocator>::operator[](Key&& key) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));
        
        if (!tmp) {
            tmp = static_cast<node_type*> (this->insert(std::move(key), mapped_type()).first);
        }

        return tmp->value.second;
    }

    /*Iterator*/
    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::begin() {
        return iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::begin() const noexcept {
        return const_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::cbegin() const noexcept {
        return const_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator map<Key, T, Compare, Allocator>::end() {
        return iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::end() const noexcept {
        return const_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::cend() const noexcept {
        return const_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::reverse_iterator map<Key, T, Compare, Allocator>::rbegin() {
        return reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_reverse_iterator map<Key, T, Compare, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(this->tree.find_min());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::reverse_iterator map<Key, T, Compare, Allocator>::rend() {
        return reverse_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_reverse_iterator map<Key, T, Compare, Allocator>::rend() const noexcept {
        return const_reverse_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator map<Key, T, Compare, Allocator>::crend() const noexcept {
        return const_reverse_iterator(this->tree.find_max());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > bool map<Key, T, Compare, Allocator>::empty() const noexcept {
        return this->tree.node_count == 0;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::size_type map<Key, T, Compare, Allocator>::max_size() const noexcept {
        return this->tree.max_size();
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::size_type map<Key, T, Compare, Allocator>::size() const noexcept {
        return this->tree.size();
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
        map<Key, T, Compare, Allocator>::insert(const value_type& value) {
        std::pair<base_type*, bool> tmp = this->insert(value);

        return std::pair<iterator, bool> (
            iterator(tmp.first),
            tmp.second
        );
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class P> 
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
        map<Key, T, Compare, Allocator>::insert(P&& value) {
        std::pair<base_type*, bool> tmp = this->insert(std::forward<P>(value));

        return std::pair<iterator, bool> (
            iterator(tmp.first),
            tmp.second
        );
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
        map<Key, T, Compare, Allocator>::insert(value_type&& value) {
        std::pair<base_type*, bool> tmp = this->insert(std::move(value));

        return std::pair<iterator, bool> (
            iterator(tmp.first),
            tmp.second
        );
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class InputIt, class = enable_if_t<!is_integral<InputIt>>> 
    void map<Key, T, Compare, Allocator>::insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            this->insert(*first);
        }
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > void map<Key, T, Compare, Allocator>::insert(std::initializer_list<value_type> ilist) {
        this->insert(ilist.begin(), ilist.end());
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::insert_return_type
        map<Key, T, Compare, Allocator>::insert(node_type&& node) {
        std::pair<base_type*, bool> tmp = this->tree.insert(std::move(node));

        return insert_return_type(
            iterator(tmp.first),
            tmp.second,
            *static_cast<node_type*>(tmp.first)
        );
    }

    template<
        class Key,
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class M>
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> 
        map<Key, T, Compare, Allocator>::insert_or_assign(const Key& key, M&& obj) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));
        bool inserted = true;

        if (tmp) {
            tmp->value.second = std::forward<M>(obj);
            inserted = false;
        } else {
            tmp = static_cast<node_type*>(this->tree.insert(key, std::forward<M>(obj)));
        }

        return std::pair<iterator, bool>(iterator(tmp), inserted);
    }

    template<
        class Key,
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class M>
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> 
        map<Key, T, Compare, Allocator>::insert_or_assign(Key&& key, M&& obj) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));
        bool inserted = true;

        if (tmp) {
            tmp->value.second = std::forward<M>(obj);
            inserted = false;
        } else {
            tmp = static_cast<node_type*>(this->tree.insert(std::move(key), std::forward<M>(obj)));
        }

        return std::pair<iterator, bool>(iterator(tmp), inserted);
    }

    template<
        class Key,
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class... Args>
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> 
        map<Key, T, Compare, Allocator>::emplace(Args&&... args) {
        std::pair<base_type*, bool> tmp = this->insert(std::forward<Args>(args)...);

        return std::pair<iterator, bool> (
            iterator(tmp.first),
            tmp.second
        );
    }

    template<
        class Key,
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class... Args>
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> 
        map<Key, T, Compare, Allocator>::try_emplace(const Key& key, Args&&... args) {
        return this->emplace(
            std::piecewise_construct,
            std::forward_tuple(k),
            std::forward_tuple(std::forward<Args>(args)...)
        );
    }

    template<
        class Key,
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class... Args>
    std::pair<typename map<Key, T, Compare, Allocator>::iterator, bool> 
        map<Key, T, Compare, Allocator>::try_emplace(Key&& key, Args&&... args) {
        return this->emplace(
            std::piecewise_construct,
            std::forward_tuple(std::move(k)),
            std::forward_tuple(std::forward<Args>(args)...)
        );
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator
        map<Key, T, Compare, Allocator>::erase(iterator pos) {
        return iterator(this->tree.erase(pos.ptr));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator
        map<Key, T, Compare, Allocator>::erase(const_iterator pos) {
        return const_iterator(this->tree.erase(pos.ptr));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::size_type
        map<Key, T, Compare, Allocator>::erase(const Key& key) {
        return this->tree.erase(key);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > void map<Key, T, Compare, Allocator>::swap(map& other)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value || 
                    || std::is_nothrow_move_assignable<Compare>::value) {
        this->tree.swap(other.tree);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::node_type
        map<Key, T, Compare, Allocator>::extract(const Key& key) {
        node_type* tmp = static_cast<node_type*>(this->tree.find(key));

        if (!tmp) {
            return node_type();
        }

        this->tree.remove_rebalancing(tmp);

        return *tmp;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::node_type
        map<Key, T, Compare, Allocator>::extract(const_iterator pos) {
        this->tree.remove_rebalancing(pos.ptr);    
        node_type* tmp = static_cast<node_type*>(pos.ptr);

        return *tmp;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class C2>
    void map<Key, T, Compare, Allocator>::merge(map<Key, T, C2, Allocator>& source) {
        for (iterator it = source.begin(); it != source.end(); ++it) {
            if (this->tree.insert_node(it.ptr)) {
                source.tree.remove_rebalancing(it.ptr);
            }
        }
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > template<class C2>
    void map<Key, T, Compare, Allocator>::merge(map<Key, T, C2, Allocator>&& source) {
        for (iterator it = source.begin(); it != source.end(); ++it) {
            if (this->tree.insert_node(it.ptr)) {
                source.tree.remove_rebalancing(it.ptr);
            }
        }
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::size_type
        map<Key, T, Compare, Allocator>::count(const Key& key) const {
        if (this->find(key)) {
            return 1;
        }

        return 0;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator
        map<Key, T, Compare, Allocator>::find(const Key& key) const {
        base_type *tmp = this->find(key);

        return iterator(tmp);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator
        map<Key, T, Compare, Allocator>::find(const Key& key) const {
        base_type *tmp = this->find(key);

        return const_iterator(tmp);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > bool map<Key, T, Compare, Allocator>::contains(const Key& key) const {
        base_type *tmp = this->find(key);
        
        return (tmp != nullptr);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > std::pair<typename map<Key, T, Compare, Allocator>::iterator,
                typename map<Key, T, Compare, Allocator>::iterator>
        map<Key, T, Compare, Allocator>::equal_range(const Key& key) const {
        return std::pair<iterator, iterator>(
            iterator(this->tree.lower_bound(key)),
            iterator(this->tree.upper_bound(key))
        );
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator
        map<Key, T, Compare, Allocator>::lower_bound(const Key& key) const {
        return iterator(this->tree.lower_bound(key));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::iterator
        map<Key, T, Compare, Allocator>::upper_bound(const Key& key) const {
        return iterator(this->tree.upper_bound(key));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator
        map<Key, T, Compare, Allocator>::lower_bound(const Key& key) const {
        return const_iterator(this->tree.lower_bound(key));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::const_iterator
        map<Key, T, Compare, Allocator>::upper_bound(const Key& key) const {
        return const_iterator(this->tree.upper_bound(key));
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::key_compare
        map<Key, T, Compare, Allocator>::key_comp() const {
        return this->tree.comp;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > typename map<Key, T, Compare, Allocator>::value_compare
        map<Key, T, Compare, Allocator>::key_comp() const {
        return value_compare(this->tree.comp);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>::map(const Compare& comp,
        const Allocator& alloc) : tree(comp, alloc) {}
    
    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>::map(const Allocator& alloc) : tree(alloc) {}

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>::map(const map& other) : tree(other.tree) {}

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>::map(map&& other) : tree(std::move(other.tree)) {}

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>&
        map<Key, T, Compare, Allocator>::operator=(map&& other) {
        this->tree = std::move(other.tree);
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > map<Key, T, Compare, Allocator>&
        map<Key, T, Compare, Allocator>::operator=(const map& other) {
        this->tree = other.tree;
    }

    template<
        class Key,
        class T,
        class Compare,
        class Allocator
    > bool operator==(const map<Key, T, Compare, Allocator>& lhs, const map<Key, T, Compare, Allocator>& rhs) {
        return equal(lhs.begin(), lhs.end(), rhs.begin());
    }
}
