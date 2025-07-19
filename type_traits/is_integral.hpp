#pragma once

namespace mystd {
    template<class T, T v>
    class integral_constant {
        static constexpr T value = v;

        using value_type = T;
        using type = Integral_constant<T, v>;

        constexpr operator value_type() const noexcept {
            return value;
        }

        constexpr value_type operator()() const noexcept {
            return value;
        }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    template<class T> struct is_integral: public false_type { };

    template<class T> struct is_integral<const T> : public is_integral<T> { };
    template<class T> struct is_integral<volatile T> : public is_integral<T> { };
    template<class T> struct is_integral<const volatile T> : public is_integral<T> { };

    template<> struct is_integral<bool> : public true_type { };
    template<> struct is_integral<char> : public true_type { };
    template<> struct is_integral<char16_t> : public true_type { };
    template<> struct is_integral<char32_t> : public true_type { };
    template<> struct is_integral<wchar_t> : public true_type { };
    template<> struct is_integral<signed char> : public true_type { };
    template<> struct is_integral<short int> : public true_type { };
    template<> struct is_integral<int> : public true_type { };
    template<> struct is_integral<long int> : public true_type { };
    template<> struct is_integral<long long int> : public true_type { };
    template<> struct is_integral<unsigned char> : public true_type { };
    template<> struct is_integral<unsigned short int> : public true_type { };
    template<> struct is_integral<unsigned int> : public true_type { };
    template<> struct is_integral<unsigned long int> : public true_type { };
    template<> struct is_integral<unsigned long long int> : public true_type { };

    template<class T> struct is_lvalue_reference : false_type {};
    template<class T> struct is_lvalue_reference<T&> : true_type {};
    template<class T>
    using is_lvalue_reference_v = is_lvalue_reference<T>::value;
}