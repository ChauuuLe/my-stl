#pragma once

namespace mystd {
    /*Constuctors*/
    template<class Iterator>
    reverse_iterator<Iterator>::reverse_iterator()
        : base_iterator() {}

    template<class Iterator>
    reverse_iterator<Iterator>::reverse_iterator(iterator_type it)
        : base_iterator(it) {}
    
    template<class Iterator>
    template<class Iter>
    reverse_iterator<Iterator>::reverse_iterator(const reverse_iterator<Iter>& rev_it)
        : base_iterator(rev_it.base()) {}
    
    /*Member functions*/
    template<class Iterator>
    typename reverse_iterator<Iterator>::iterator_type  reverse_iterator<Iterator>::base() const {
        return this->base_iterator;
    }

    /*Post and pre + - operator*/
    template<class Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator++(void)  {
        --this->base_iterator;

        return *this;
    }

    template<class Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator--(void)  {
        ++this->base_iterator;

        return *this;
    }

    template<class Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator++(int)  {
        reverse_iterator temp = *this;
        --this->base_iterator;
        
        return temp;
    }

    template<class Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator--(int)  {
        reverse_iterator temp = *this;
        ++this->base_iterator;
        
        return temp;
    }

    /*Arithmetic operator*/    
    template<class Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator+=(
            typename reverse_iterator<Iterator>::difference_type n)  {
        this->base_iterator -= n;
        
        return *this;
    }

    template<class Iterator>
    reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator-=(
            typename reverse_iterator<Iterator>::difference_type n)  {
        this->base_iterator += n;
        
        return *this;
    }

    template<class Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator+(
            typename reverse_iterator<Iterator>::difference_type n)  const {
        return reverse_iterator(this.base() - n);
    }

    template<class Iterator>
    reverse_iterator<Iterator> reverse_iterator<Iterator>::operator-(
            typename reverse_iterator<Iterator>::difference_type n)  const {
        return reverse_iterator(this.base() + n);
    }

    template<class Iterator>
    reverse_iterator<Iterator> operator+(
            const reverse_iterator<Iterator>& it,
            typename reverse_iterator<Iterator>::difference_type n
        )  {
        return reverse_iterator(it.base() - n);
    }

    template<class Iterator>
    reverse_iterator<Iterator> operator-(
            const reverse_iterator<Iterator>& it,
            typename reverse_iterator<Iterator>::difference_type n
        )  {
        return reverse_iterator(it.base() + n);
    }

    /*Dereference operator*/
    template<class Iterator>
    typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::operator*() const{
        Iterator base_iter = this->base_iterator;

        return *(--base_iter);
    }

    template<class Iterator>
    typename reverse_iterator<Iterator>::pointer reverse_iterator<Iterator>::operator->() const{
        Iterator base_iter = this->base_iterator;

        return (base_iter - 1);
    }

    template<class Iterator>
    typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::operator[](
            typename reverse_iterator<Iterator>::difference_type n) const{
        Iterator base_iter = this->base_iterator;

        return base_iter[-n - 1];
    }

    /*Compare operator*/
    template<class Iter1, class Iter2>
    bool operator==(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() == rhs.base());
    }
    
    template<class Iter1, class Iter2>
    bool operator!=(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() != rhs.base());
    }
    
    template<class Iter1, class Iter2>
    bool operator<(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() < rhs.base());
    }
    template<class Iter1, class Iter2>
    bool operator<=(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() <= rhs.base());
    }

    template<class Iter1, class Iter2>
    bool operator>(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() > rhs.base());
    }
    
    template<class Iter1, class Iter2>
    bool operator>=(const reverse_iterator<Iter1>& lhs,
                    const reverse_iterator<Iter2>& rhs) {
        return (lhs.base() >= rhs.base());
    }
}