#pragma once

#include "rb_tree_node.hpp"
#include <cstddef>
#include <iterator>
#include "../../memory/uninitialized_memory.hpp"

namespace mystd {
    template<class Key>
    struct rb_tree_iterator {
        using node_type = typename rb_tree_node<Key>;
        using base_node_type = rb_tree_node_base;
        
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = Key*;
        using reference = Key&;
        using iterator_category = std::bidirectional_iterator_tag;

        base_node_type *ptr;

        rb_tree_iterator(const rb_tree_iterator& it);
        rb_tree_iterator(base_node_type *ptr);
        rb_tree_iterator();

        ~rb_tree_iterator() = default;

        rb_tree_iterator& operator=(const rb_tree_iterator& rhs);

        operator rb_tree_iterator<const Key>() const;

        rb_tree_iterator& operator++(void);
        rb_tree_iterator operator++(int);
        rb_tree_iterator& operator--(void);
        rb_tree_iterator operator--(int);

        bool operator==(const rb_tree_iterator& rhs) const;
        bool operator!=(const rb_tree_iterator& rhs) const;
        bool operator<(const rb_tree_iterator& rhs) const;
        bool operator>(const rb_tree_iterator& rhs) const;
        bool operator<=(const rb_tree_iterator& rhs) const;
        bool operator>=(const rb_tree_iterator& rhs) const;

        reference operator*() const;
        pointer operator->() const;
    };
}

#include "rb_tree_iterator.tpp"
