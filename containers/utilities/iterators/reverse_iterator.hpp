#pragma once

#include "iterator_traits.hpp"

namespace mystd {
    template<class Iterator>
    class reverse_iterator {
    private:
        Iterator base_iterator;
    public:
        using iterator_type = Iterator;
        using iterator_category = typename iterator_traits<Iterator>::iterator_category;
        using value_type = typename iterator_traits<Iterator>::value_type;
        using difference_type = typename iterator_traits<Iterator>::difference_type;
        using pointer = typename iterator_traits<Iterator>::pointer;
        using reference = typename iterator_traits<Iterator>::reference;
        
        reverse_iterator();

        explicit reverse_iterator(iterator_type it);

        template<class Iter>
        reverse_iterator(const reverse_iterator<Iter> &rev_it);

        iterator_type base() const;

        reverse_iterator& operator++(void);
        reverse_iterator& operator--(void);
        reverse_iterator operator++(int);
        reverse_iterator operator--(int);
        reverse_iterator operator+(difference_type n) const;
        reverse_iterator operator-(difference_type n) const;
        reverse_iterator& operator+=(difference_type n);
        reverse_iterator& operator-=(difference_type n);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type n) const;

        template<class Iter1, class Iter2>
        bool operator==(const reverse_iterator<Iter1>& lhs,
                        const reverse_iterator<Iter2>& rhs);
        
        template<class Iter1, class Iter2>
        bool operator!=(const reverse_iterator<Iter1>& lhs,
                        const reverse_iterator<Iter2>& rhs);
        
        template<class Iter1, class Iter2>
        bool operator<(const reverse_iterator<Iter1>& lhs,
                        const reverse_iterator<Iter2>& rhs);

        template<class Iter1, class Iter2>
        bool operator<=(const reverse_iterator<Iter1>& lhs,
                        const reverse_iterator<Iter2>& rhs);

        template<class Iter1, class Iter2>
        bool operator>(const reverse_iterator<Iter1>& lhs,
                       const reverse_iterator<Iter2>& rhs);

        template<class Iter1, class Iter2>
        bool operator>=(const reverse_iterator<Iter1>& lhs,
                        const reverse_iterator<Iter2>& rhs);
    };

    #include "reverse_iterator.tpp"
}
