#pragma once

namespace mystd {
    /*Capacity*/
    template<class T, class Allocator>
    bool vector<T, Allocator>::empty() const {
        return (this->nelem == 0);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const {
        return this->nelem;
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const {
        return this->cap;
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const {
        const size_type allocator_limit = std::allocator_traits<Allocator>::max_size(*this);
        const size_type sys_limit = std::numeric_limits<difference_type>::max();

        return (allocator_limit < sys_limit) ? allocator_limit : sys_limit;
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::shrink_to_fit() {
        if (!this->elems || this->cap == this->nelem) {
            return;
        }

        static_assert(std::is_copy_constructible_v<value_type> || std::is_nothrow_move_constructible_v<value_type>,
                    "Elements should be copy constructible or move constructible with no throw");
        
        size_type new_cap = this->nelem;
        pointer new_buf = nullptr;
        try {
            new_buf = std::allocator_traits<Allocator>::allocate(*this, new_cap);

            if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                uninitialized_move_construct(this->begin(), this->nelem, iterator(new_buf));      
            } else {
                uninitialized_copy_construct(this->begin(), this->nelem, iterator(new_buf));
            }
        } catch(...) {
            if (new_buf) {
                std::allocator_traits<Allocator>::deallocate(*this, new_buf, new_cap);
            }
            throw;
        }

        std::destroy(this->begin(), this->end());
        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->cap);
        this->cap = new_cap;
        this->elems = new_buf;
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::reserve(size_type new_cap) {

        if (new_cap <= this->cap) {
            return;
        }
        static_assert(std::is_copy_constructible_v<value_type> || std::is_nothrow_move_constructible_v<value_type>,
                    "Elements should be copy constructible or move constructible with no throw");

        pointer new_buf = nullptr;
        try {
            new_buf = std::allocator_traits<Allocator>::allocate(*this, new_cap);

            if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                uninitialized_move_construct(this->begin(), this->nelem, iterator(new_buf));      
            } else {
                uninitialized_copy_construct(this->begin(), this->nelem, iterator(new_buf));
            }
        } catch(...) {
            if (new_buf) {
                std::allocator_traits<Allocator>::deallocate(*this, new_buf, new_cap);
            }
            throw;
        }

        std::destroy(this->begin(), this->end());
        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->cap);
        this->cap = new_cap;
        this->elems = new_buf;
    }

    /*Iterators*/
    template<class T, class Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() {
        return iterator(this->elems);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const {
        return const_iterator(this->elems);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::end() {
        return iterator(this->elems + this->nelem);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const {
        return const_iterator(this->elems + this->nelem);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() {
        return reverse_iterator(this->end());
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const {
        return const_reverse_iterator(this->end());
    }
    
    template<class T, class Allocator>
    typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() {
        return reverse_iterator(this->begin());
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const {
        return const_reverse_iterator(this->begin());
    }
    
    /*Member function*/
    template<class T, class Allocator>
    typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const {
        return allocator_type(*static_cast<*allocator_type>(this));
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::assign(size_type count, const value_type& value) {
        static_assert(std::is_copy_assignable_v<value_type> && std::is_copy_constructible_v<value_type>,
            "Value should copy assignable and copy constructable");

        if (count <= this->cap) {
            if (count <= this->nelem) {
                for (size_type i = 0; i < count; i++) {
                    this->elems[i] = value;
                }

                for (size_type i = count; i < this->nelem; i++) {
                    std::allocator_traits<Allocator>::destroy(*this, this->elems + i);
                }
            } else {
                for (size_type i = 0; i < this->nelem; i++) {
                    this->elems[i] = value;
                }

                for (size_type i = this->nelem; i < count; i++) {
                    std::allocator_traits<Allocator>::construct(*this, this->elems + i, value);
                }
            }

            this->nelem = count;
            return;
        }

        pointer new_buf = nullptr;
        size_type i = 0;
        try {
            new_buf = std::allocator_traits<Allocator>::allocate(*this, count);
            for (; i < count; i++) {
                std::allocator_traits<Allocator>::construct(*this, new_buf + i, value);
            }
        } catch(...) {
            if (new_buf) {
                for (size_type j = 0; j < i; j++) {
                    std::allocator_traits<Allocator>::destroy(*this, new_buf + j);
                }

                std::allocator_traits<Allocator>::deallocate(*this, new_buf, count);
            }

            throw;
        }

        std::destroy(this->begin(), this->end());
        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->cap);
        this->nelem = count;
        this->cap = count;
        this->elems = new_buf;
    }

    /*Ctors*/
    template<class T, class Allocator>
    vector<T, Allocator>::vector(): Allocator(Allocator()), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator>
    vector<T, Allocator>::vector(const Allocator& alloc): Allocator(alloc), elems(nullptr), nelem(0), cap(0) {}

    template<class T, class Allocator>
    vector<T, Allocator>::vector(size_type count, const Allocator& alloc)
        : Allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {

        try {
            this->elems = std::allocator_traits<Allocator>::allocate(*this, count);
            this->cap = count;
            uninitialized_value_construct(count, this->begin());
            this->nelem = count;
        } catch(...) {
            if (this->elems) {
                std::allocator_traits<Allocator>::deallocate(*this, this->elems, count);
            }
            throw;
        }
    }

    template<class T, class Allocator>
    vector<T, Allocator>::vector(size_type count, const value_type& value, const Allocator& alloc)
        : Allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {

        try {
            this->elems = std::allocator_traits<Allocator>::allocate(*this, count);
            this->cap = count;
            uninitialized_copy_value_construct(value, count, this->begin());
            this->nelem = count;
        } catch(...) {
            if (this->elems) {
                std::allocator_traits<Allocator>::deallocate(*this, this->elems, count);
            }      
            throw;
        }
    }

    template<class T, class Allocator>
    template<class InputIt, class = mystd::enable_if_t<!is_integral<InputIt>::value>>
    vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator& alloc)
        : Allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)) {
        
        try {
            this->nelem = std::distance(first, last);
            this->elems = std::allocator_traits<Allocator>::allocate(*this, this->nelem);
            this->cap = this->nelem;
            uninitialized_copy_construct(first, this->size(), this->begin());
        } catch(...) {
            if (this->elems)
                std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->nelem);
            
            throw;
        }
    }

    template<class T, class Allocator>
    vector<T, Allocator>::vector(const vector& other)
        : Allocator(
            std::allocator_traits<Allocator>::select_on_container_copy_construction(*static_cast<Allocator*>(&other))
        ) {
        try {
            this->nelem = other.nelem;
            this->elems = std::allocator_traits<Allocator>::allocate(*this, this->nelem);
            this->cap = this->nelem;   
            uninitialized_copy_construct(other.cbegin(), this->size(), this->begin());
        } catch(...) {
            if (this->elems)
                std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->nelem);
            throw;
        }
    }

    template<class T, class Allocator>
    vector<T, Allocator>::vector(vector&& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value ||
                                                            std::is_nothrow_move_constructible_v<Allocator>)
        : Allocator(std::move(*static_cast<Allocator*>(&other))),
            nelem(other.nelem), cap(other.cap), elems(other.elems) {
        other.elems = nullptr;
        other.nelem = 0;
        other.cap = 0;
    }

    template<class T, class Allocator>
    vector<T, Allocator>::vector(std::initializer_list<value_type> init, const Allocator& alloc)
        : Allocator(std::allocator_traits<Allocator>::select_on_container_copy_construction(alloc)),
          nelem(init.size()), cap(init.size()) {

        try {
            this->elems = std::allocator_traits<Allocator>::allocate(*this, this->nelem);
            uninitialized_copy_construct(init.begin(), this->size(), this->begin());
        } catch(...) {
            if (this->elems)
                std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->nelem);
            throw;
        }
    }

    /*Destructor*/
    template<class T, class Allocator>
    vector<T, Allocator>::~vector() noexcept(std::is_nothrow_destructible_v<value_type>) {
        std::destroy(this->begin(), this->end());
        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->capacity());
    }

    /*Element access*/
    template<class T, class Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos) {
        if (pos >= this->size()) {
            throw std::out_of_range("Access out of range for std vector");
        } 
        return *(this->elems + pos);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const {
        if (pos >= this->size()) {
            throw std::out_of_range("Access out of range for std vector");
        } 
        return *(this->elems + pos);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type pos) {
        return *(this->elems + pos);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type pos) const {
        return *(this->elems + pos);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
        return *(this->elems);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
        return *(this->elems);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
        return *(this->elems + this->nelem - 1);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
        return *(this->elems + this->nelem - 1);
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::pointer vector<T, Allocator>::data() {
        return this->elems;
    }

    template<class T, class Allocator>
    typename vector<T, Allocator>::const_pointer vector<T, Allocator>::data() const {
        return this->elems;
    }

    /*Modifiers*/
    template<class T, class Allocator>
    void vector<T, Allocator>::clear() noexcept(std::is_nothrow_destructible_v<value_type>) {
        std::destroy(this->begin(), this->end());
        this->nelem = 0;
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::push_back(const T& value) {
        static_assert(std::is_copy_assignable_v<value_type> && std::__is_copy_insertable<Allocator>::value,
            "Value should be copy assignable and copy insertable");

        size_type new_size = this->size() + 1;
        if (new_size <= this->capacity()) {
            std::allocator_traits<Allocator>::construct(*this, this->elems + size(), value);
            this->nelem = new_size;
            return;
        }

        pointer new_buf = nullptr;
        size_type new_cap = this->get_next_cap();
        size_type i = 0;

        try { 
            new_buf = std::allocator_traits<Allocator>::allocate(*this, new_cap);

            if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                for (; i < this->nelem; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i, std::move(*(this->elems + i)));
                }    
            } else {
                for (; i < this->nelem; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i, *(this->elems + i));
                }
            }
            
            std::allocator_traits<Allocator>::construct(*this, new_buf + this->nelem, value);
        } catch (...) {
            if (new_buf) {
                for (size_type j = 0; j < i; j++) {
                    std::allocator_traits<Allocator>::destroy(*this, new_buf + j);
                }
                std::allocator_traits<Allocator>::deallocate(*this, new_buf, new_cap);
            }
            throw;
        }

        for (size_type j = 0; j < this->nelem; ++j) {
            std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
        }

        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->capacity());
        this->nelem = new_size;
        this->elems = new_buf;
        this->cap = new_cap;
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::push_back(T&& value) {
        this->emplace_back(std::move(value));
    } 

    template<class T, class Allocator>
    template<class... Args>
    void vector<T, Allocator>::emplace_back(Args&&... args) {
        static_assert(std::is_move_constructible_v<value_type> || std::__is_move_insertable<Allocator>::value,
            "value_type must be move insertable and move assignable");
        
        size_type new_size = this->size() + 1;
        if (new_size <= this->capacity()) {
            std::allocator_traits<Allocator>::construct(*this, this->elems + size(), std::forward<Args>(args)...);
            this->nelem = new_size;
            return;
        }

        pointer new_buf = nullptr;
        size_type new_cap = this->get_next_cap();
        size_type i = 0;

        try { 
            new_buf = std::allocator_traits<Allocator>::allocate(*this, new_cap);

            if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                for (; i < this->nelem; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i, std::move(*(this->elems + i)));
                }    
            } else {
                for (; i < this->nelem; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i, *(this->elems + i));
                }
            }
            
            std::allocator_traits<Allocator>::construct(*this, new_buf + this->nelem, std::forward<Args>(args)...);
        } catch (...) {
            if (new_buf) {
                for (size_type j = 0; j < i; j++) {
                    std::allocator_traits<Allocator>::destroy(*this, new_buf + j);
                }
                std::allocator_traits<Allocator>::deallocate(*this, new_buf, new_cap);
            }
            throw;
        }

        for (size_type j = 0; j < this->nelem; ++j) {
            std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
        }

        std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->capacity());
        this->nelem = new_size;
        this->elems = new_buf;
        this->cap = new_cap;
    }

    template<class T, class Allocator>
    void vector<T, Allocator>::pop_back() {
        std::allocator_traits<Allocator>::destroy(*this, this->elems + this->nelem - 1);
        --this->nelem;
    }

    template<class T, class Allocator>
    void vector<T,Allocator>::resize(size_type count) {
        if (count == this->nelem) {
            return;
        }

        if (count < this->nelem) {
            std::destroy(iterator(this->elems + count), this->end());
            this->nelem = count;
        } else {
            if (count <= this->cap) {
                size_type i = this->nelem;
                try {
                    for (i; i < count; i++) {
                        std::allocator_traits<Allocator>::construct(*this, this->elems + i);
                    }
                    this->nelem = count;
                } catch(...) {
                    for (size_type j = this->nelem; j < i; j++) {
                        std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
                    }
                    throw;
                }
                return;
            }
            static_assert(std::is_nothrow_move_constructible_v<T> || std::is_copy_constructible_v<T>,
                "resize requires T to be nothrow-move-constructible or copy-constructible");

            pointer new_buf = nullptr;
            size_type i = 0;

            try {
                new_buf = std::allocator_traits<Allocator>::allocate(*this, count);

                if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                    for (; i < this->nelem; i++) {
                        std::allocator_traits<Allocator>::construct(*this, new_buf + i, std::move(*(this->elems + i)));
                    }    
                } else {
                    for (; i < this->nelem; i++) {
                        std::allocator_traits<Allocator>::construct(*this, new_buf + i, *(this->elems + i));
                    }
                }

                for (i; i < count; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i);
                }
                
            } catch(...) {
                if (new_buf) {
                    for (size_type j = 0; j < i; j++) {
                        std::allocator_traits<Allocator>::destroy(*this, new_buf + j);
                    }
                    std::allocator_traits<Allocator>::deallocate(*this, new_buf, count);
                }
                throw;
            }

            for (size_type j = 0; j < this->nelem; ++j) {
                std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
            }

            std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->capacity());

            this->cap = count;
            this->nelem = count;
            this->elems = new_buf;
        }
    }

    template<class T, class Allocator>
    void vector<T,Allocator>::resize(size_type count, const T& value) {
        if (count == this->nelem) {
            return;
        }

        if (count < this->nelem) {
            std::destroy(iterator(this->elems + count), this->end());
            this->nelem = count;
        } else {
            if (count <= this->cap) {
                size_type i = this->nelem;
                try {
                    for (i; i < count; i++) {
                        std::allocator_traits<Allocator>::construct(*this, this->elems + i);
                    }
                    this->nelem = count;
                } catch(...) {
                    for (size_type j = this->nelem; j < i; j++) {
                        std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
                    }
                    throw;
                }
                return;
            }
            static_assert(std::is_nothrow_move_constructible_v<T> || std::is_copy_constructible_v<T>,
                "resize requires T to be nothrow-move-constructible or copy-constructible");

            pointer new_buf = nullptr;
            size_type i = 0;

            try {
                new_buf = std::allocator_traits<Allocator>::allocate(*this, count);

                if constexpr(std::is_nothrow_move_constructible_v<value_type> ||
                            !std::is_copy_constructible_v<value_type>) {
                    for (; i < this->nelem; i++) {
                        std::allocator_traits<Allocator>::construct(*this, new_buf + i, std::move(*(this->elems + i)));
                    }    
                } else {
                    for (; i < this->nelem; i++) {
                        std::allocator_traits<Allocator>::construct(*this, new_buf + i, *(this->elems + i));
                    }
                }

                for (i; i < count; i++) {
                    std::allocator_traits<Allocator>::construct(*this, new_buf + i, value);
                }
                
            } catch(...) {
                if (new_buf) {
                    for (size_type j = 0; j < i; j++) {
                        std::allocator_traits<Allocator>::destroy(*this, new_buf + j);
                    }
                    std::allocator_traits<Allocator>::deallocate(*this, new_buf, count);
                }
                throw;
            }

            for (size_type j = 0; j < this->nelem; ++j) {
                std::allocator_traits<Allocator>::destroy(*this, this->elems + j);
            }

            std::allocator_traits<Allocator>::deallocate(*this, this->elems, this->capacity());

            this->cap = count;
            this->nelem = count;
            this->elems = new_buf;
        }
    }

    template<class T, class Allocator>
    void vector<T,Allocator>::swap(vector& other) 
                noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value  || 
                    std::allocator_traits<Allocator>::is_always_equal::value) {
        std::swap(this->elems, other.elems);
        std::swap(this->cap, other.cap);
        std::swap(this->nelem, other.nelem);

        if constexpr(std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(*this, other.get_allocator());
        }
    }

    /*Non-member functions*/
    template<class T, class Allocator>
    bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class T, class Allocator>
    bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return lhs.size() != rhs.size() || equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class T, class Allocator>
    bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template<class T, class Allocator>
    void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs) {
        lhs.swap(rhs);
    }
}