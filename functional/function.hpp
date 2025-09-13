#pragma once
#include <type_traits>
#include <utility>
#include <new>
#include <exception>
#include <typeinfo>
#include <cstddef>
#include <cstring>

namespace mystd {
    template<class T>
    class function<T>;

    template<class R, class... Args>
    class function<R(Args...)> {
        static constexpr size_t SBO = 2 * sizeof(void*);
        using byte = unsigned char;

        union _Any_data {
            void* _ptr;
            alignas(std::max_align_t) byte _buffer[SBO];
        };

        enum class _Op { get_type, get_ptr, clone, move, destroy };

        using _Invoker = R(*)(const _Any_data&, Args&&...);
        using _Manager = void(*)(_Op, _Any_data&, _Any_data&, void** /*ret_ptr*/, const std::type_info** /*ret_type*/);

        _Any_data _functor;
        _Invoker _invoker = nullptr;
        _Manager _manager = nullptr;

        template<class F>
        static constexpr bool _stored_locally() noexcept {
            using D = std::decay_t<F>;

            return std::is_trivially_copyable_v<D> && sizeof(D) <= SBO && alignof(D) <= std::max_align_t; 
        }

        template<class F>
        static R _invoke_heap(const _Any_data& functor, Args&&... args) {
            const *F ptr = static_cast<const F*> (functor._ptr);
            if constexpr(std::is_void_v<R>) {
                (*ptr) (std::forward<Args>(args)...);
                return;
            } else {
                return (*ptr) (std::forward<Args>(args)...);
            }
        }

        template<class F>
        static R _invoke_local(const _Any_data& functor, Args&&... args) {
            const *F ptr = reinterpret_cast<const F*> (functor._buffer);
            if constexpr(std::is_void_v<R>) {
                (*ptr) (std::forward<Args>(args)...);
                return;
            } else {
                return (*ptr) (std::forward<Args>(args)...);
            }
        }

        template<class F>
        static void _manage_local(_Op op, _Any_data& out, _Any_data& inout, void** ret_ptr, const std::type_info** ret_type) {
            using D = std::decay_t<F>;

            switch (op) {
                case _Op::get_type: {
                    if (ret_type) {
                        *ret_type = &typeid(D);
                    }
                    break;
                }  
                case _Op::get_ptr: {
                    if (ret_ptr) {
                        *ret_ptr = const_cast<D*>(reinterpret_cast<const D*>(inout._buffer));
                    }
                    break;
                }
                case _Op::clone: {
                    const D* src = reinterpret_cast<D*>(inout._buffer);
                    ::new (static_cast<void*>(out._buffer)) D(*src);
                    break;
                }
                case _Op::move: {
                    D* src = reinterpret_cast<D*>(inout._buffer);
                    ::new (static_cast<void*>(out._buffer)) D(std::move(*src));
                    src->~D();
                    break;
                }
                case _Op::destroy: {
                    D* obj = reinterpret_cast<D*>(inout._buffer);
                    obj->~D();
                    break;
                }
            }
        }

        template<class F>
        static void _manage_heap(_Op op, _Any_data& out, _Any_data& inout, void** ret_ptr, const std::type_info** ret_type) {
            using D = std::decay_t<F>;

            switch (op) {
                case _Op::get_type: {
                    if (ret_type) {
                        *ret_type = &typeid(D);
                    }
                    break;
                }  
                case _Op::get_ptr: {
                    if (ret_ptr) {
                        *ret_ptr = inout._ptr;
                    }
                    break;
                }
                case _Op::clone: {
                    const D* src = static_cast<const D*>(inout._ptr);
                    out._ptr = new D(*src);
                    break;
                }
                case _Op::move: {
                    out._ptr = inout._ptr;
                    inout._M_p = nullptr;
                    break;
                }
                case _Op::destroy: {
                    delete static_cast<D*>(inout._ptr); 
                    inout._ptr = nullptr;
                    break;
                }
            }
        }

        template<class F>
        static void _construct(F&& f) {
            using D = std::decay_t<F>;
            static_assert(std::is_trivially_copy_assignable_v<D>);
            static_assert(std::is_invocable_r<R, D&, Args...>::value);

            if constexpr(_stored_locally<D>()) {
                ::new (static_cast<void*>(&this->_functor._buffer)) D(std::forward<F>(f));
                this->_invoker = &_invoke_local<D>;
                this->_manager = &_manage_local<D>;
            } else {
                this->_functor._ptr = new D(std::forward<F>(f));
                this->_invoker = &_invoke_heap<D>;
                this->_manager = &_manage_heap<D>;
            }
        }

        static bool is_empty_fp(R(*fp)(Args&&...)) {
            return fp == nullptr;
        }

        void _M_reset() noexcept {
            if (_manager) {
                _manager(_Op::destroy, _functor, _functor, nullptr, nullptr);
                _invoker = nullptr;
                _manager = nullptr;
            }
        }
        void _clear() noexcept {
            _invoker = nullptr;
            _manager = nullptr;
            _functor._M_p = nullptr;
        }
    public:
        function() noexcept = default;
        function(std::nullptr_t) noexcept : function() {}

        function(R(*fp)(Args...)) {
            if (!is_empty_fp(fp)) {
                _construct(fp);
            }
        }

        template<class F,
             class = std::enable_if_t<!std::is_same_v<std::decay_t<F>, function>>>
        function(F&& f) {
            _M_construct(std::forward<F>(f));
        }

        function(const function& other) {
            if (!other) return;
            _invoker = other._invoker;
            _manager = other._manager;
            
            _manager(_Op::clone, _functor, const_cast<_Any_data&>(other._functor), nullptr, nullptr);
        }

        function(function&& other) {
            if (!other) return;
            _invoker = other._invoker;
            _manager = other._manager;
            
            _manager(_Op::move, _functor, const_cast<_Any_data&>(other._functor), nullptr, nullptr);
            other._clear();
        }

        R operator()(Args... args) const {
            return _invoker(_functor, std::forward<Args>(args)...);
        }

        function& operator=(std::nullptr_t) noexcept { _M_reset(); return *this; }

        function& operator=(const function& rhs) {
            if (this == &rhs) return *this;
            function tmp(rhs);
            swap(tmp);
            return *this;
        }

        function& operator=(function&& rhs) noexcept {
            if (this == &rhs) return *this;
            _M_reset();
            if (!rhs) return *this;
            _invoker = rhs._invoker;
            _manager = rhs._manager;
            _manager(_Op::move, _functor, rhs._functor, nullptr, nullptr);
            rhs._M_clear();
            return *this;
        }

        // generic assignment
        template<class F,
                class = std::enable_if_t<!std::is_same_v<std::decay_t<F>, function>>>
        function& operator=(F&& f) {
            function tmp(std::forward<F>(f));
            swap(tmp);
            return *this;
        }

        void swap(function& other) noexcept {
            using std::swap;
            swap(_functor, other._functor);
            swap(_invoker, other._invoker);
            swap(_manager, other._manager);
        }

        void reset() noexcept { _M_reset(); }

        // --- observers ---
        explicit operator bool() const noexcept { 
            return _invoker != nullptr;
        }

        const std::type_info& target_type() const noexcept {
            if (!_manager) return typeid(void);
            const std::type_info* ti = nullptr;
            _manager(_Op::get_type, const_cast<_Any_data&>(_functor),
                    const_cast<_Any_data&>(_functor), nullptr, &ti);
            return *ti;
        }

        template<class T> 
        T* target() noexcept {
            if (!_manager || target_type() != typeid(T)) return nullptr;
            void* p = nullptr;
            _manager(_Op::get_ptr, _functor, _functor, &p, nullptr);
            return static_cast<T*>(p);
        }

        template<class T> const T* target() const noexcept {
            if (!_manager || target_type() != typeid(T)) return nullptr;
            void* p = nullptr;
            _manager(_Op::get_ptr, const_cast<_Any_data&>(_functor),
                    const_cast<_Any_data&>(_functor), &p, nullptr);
            return static_cast<const T*>(p);
        }
    };
}