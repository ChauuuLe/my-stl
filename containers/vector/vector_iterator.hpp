#pragma once

#include<cstddef>
#include<iterator>

namespace mystd {
    template<class T>
    class vector_iterator {
    private:
        T *ptr;
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        vector_iterator(const vector_iterator& v_it);
        vector_iterator(value_type *ptr);
        vector_iterator();

        ~vector_iterator() = default;

        vector_iterator& operator=(const vector_iterator& rhs);

        operator vector_iterator<const T>() const;

        vector_iterator& operator++(void);
        vector_iterator operator++(int);
        vector_iterator& operator--(void);
        vector_iterator operator--(int);

        bool operator==(const vector_iterator& rhs) const;
        bool operator!=(const vector_iterator& rhs) const;
        bool operator<(const vector_iterator& rhs) const;
        bool operator>(const vector_iterator& rhs) const;
        bool operator<=(const vector_iterator& rhs) const;
        bool operator>=(const vector_iterator& rhs) const;

        reference operator*() const;
        reference operator[](difference_type n) const;
        pointer operator->() const;

        vector_iterator	operator+(difference_type n) const;
	    vector_iterator	operator-(difference_type n) const;
	    difference_type	operator-(const vector_iterator& rhs) const;

        vector_iterator& operator+=(difference_type n);
        vector_iterator& operator-=(difference_type n);
    };
}

#include "vector_iterator.tpp"
