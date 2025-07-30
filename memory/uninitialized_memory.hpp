#pragma once
#include <memory>
#include <iterator>
#include "../type_traits/is_integral.hpp"
#include "../type_traits/enable_if.hpp"
#include <type_traits>
#include <utility>

namespace mystd {
    template<class T>
    enable_if_t<std::is_object_v<T>, T*> addressof(T& value) {
        return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(value)
            )
        );
    }

    template<class T>
    enable_if_t<!std::is_object_v<T>, T*> addressof(T& value) {
        return &value;
    }

    template<class T>
    T* addressof(const T&& value) = delete;

    template<class InputIt, class Size, class ForwardIt>
    ForwardIt uninitialized_copy_construct(InputIt first, Size count, ForwardIt d_first) {
        ForwardIt current = d_first;
        using T = typename std::iterator_traits<ForwardIt>::value_type;

        try {
            for (; count > 0; --count, ++current, ++first) {
                ::new (static_cast<void*>(addressof(*current))) T(*first); 
            }

            return current;
        } catch(...) {
            std::destroy(d_first, current);
            throw;
        }
    }

    template<class T, class Size, class ForwardIt>
    ForwardIt uninitialized_copy_value_construct(const T& value, Size count, ForwardIt d_first) {
        ForwardIt current = d_first;
        using U = typename std::iterator_traits<ForwardIt>::value_type;

        try {
            for (; count > 0; --count, ++current) {
                ::new (static_cast<void*>(addressof(*current))) U(value); 
            }

            return current;
        } catch(...) {
            std::destroy(d_first, current);
            throw;
        }
    }

    template<class Size, class ForwardIt>
    void uninitialized_value_construct(Size count, ForwardIt d_first) {
        ForwardIt current = d_first;
        using T = typename std::iterator_traits<ForwardIt>::value_type;

        try {
            for (; count > 0; --count, ++current) {
                ::new (static_cast<void*>(addressof(*current))) T(); 
            }
        } catch(...) {
            std::destroy(d_first, current);
            throw;
        }
    }

    template<class InputIt, class Size, class ForwardIt>
    ForwardIt uninitialized_move_construct(InputIt first, Size count, ForwardIt d_first) {
        ForwardIt current = d_first;
        using T = typename std::iterator_traits<ForwardIt>::value_type;

        try {
            for (; count > 0; --count, ++current, ++first) {
                ::new (static_cast<void*>(addressof(*current))) T(std::move(*first)); 
            }

            return current;
        } catch(...) {
            std::destroy(d_first, current);
            throw;
        }
    }

    template<class Size, class ForwardIt>
    void uninitialized_default_construct(Size count, ForwardIt d_first) {
        ForwardIt current = d_first;
        using T = typename std::iterator_traits<ForwardIt>::value_type;

        try {
            for (; count > 0; --count, ++current) {
                ::new (static_cast<void*>(addressof(*current))) T; 
            }
        } catch(...) {
            std::destroy(d_first, current);
            throw;
        }
    }
}