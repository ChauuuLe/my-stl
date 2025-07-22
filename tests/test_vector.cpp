#include "../containers/vector/vector.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <string>
#include "../type_traits/is_integral.hpp"

using mystd::vector;

// A small helper type for emplace_back tests
struct Emplaceable {
    int x;
    std::string s;
    Emplaceable(int a, const std::string& b) : x(a), s(b) {}
};

static void test_constructors_and_empty_size() {
    // default ctor
    vector<int> v1;
    assert(v1.empty() && v1.size() == 0);

    // allocator ctor (same as default)
    vector<int> v1a(std::allocator<int>{});
    assert(v1a.empty());

    // count ctor (default-initializes ints to 0)
    vector<int> v2(5);
    assert(v2.size() == 5);
    std::cout << v2.capacity() << std::endl;
 
    for (auto &x : v2) {
        std::cout << x << " v2 " << &x << std::endl;
    }

    // fill ctor
    vector<int> v3(4, 7);
    assert(v3.size() == 4);
    for (auto x : v3) assert(x == 7);

    // initializer-list ctor
    vector<int> v4{1,2,3,4};
    assert(v4.size()==4 && v4[0]==1 && v4.back()==4);

    // range ctor
    int arr[] = {5,6,7};
    //std::cout << bool(mystd::is_integral<int*>{}) << std::endl;
    vector<int> v5(std::begin(arr), std::end(arr));
    assert(v5.size()==3 && v5[1]==6);

    // copy ctor
    vector<int> v6(v5);
    assert(v6.size()==3 && v6[2]==7);

    // move ctor
    vector<int> v7(std::move(v6));
    assert(v7.size()==3);
    assert(v6.size()==0);  // moved-from should be empty by your impl
}

static void test_element_access() {
    vector<int> v{10,20,30};
    // operator[]
    assert(v[0]==10 && v[2]==30);

    // at() valid
    assert(v.at(1)==20);

    // at() out-of-range
    bool threw = false;
    try { v.at(3); }
    catch (const std::out_of_range&) { threw=true; }
    assert(threw);

    // front/back
    assert(v.front()==10 && v.back()==30);

    // data()
    int* p = v.data();
    p[1] = 99;
    assert(v[1]==99);
}

static void test_iterators() {
    vector<int> v{1,2,3,4};
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    assert(sum == 10);

    // const_iterator
    const vector<int>& cv = v;
    sum = 0;
    for (auto it = cv.cbegin(); it != cv.end(); ++it)
        sum += *it;
    assert(sum == 10);

    // reverse_iterator
    std::string s;
    for (auto rit = v.rbegin(); rit != v.rend(); ++rit)
        s += char('0' + *rit);
    assert(s == "4321");
}

static void test_capacity_and_reserve_shrink() {
    vector<int> v;
    assert(v.capacity() >= 0);

    v.reserve(20);
    assert(v.capacity() >= 20);

    v.push_back(1);
    v.push_back(2);
    auto oldcap = v.capacity();
    v.shrink_to_fit();
    assert(v.capacity() == v.size());

    // shrink_to_fit may or may not reduce capacity if implementation chooses,
    // but capacity() must be >= size() always
    assert(v.capacity() >= v.size());
}

static void test_modifiers_push_pop_clear_assign() {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.emplace_back(3);
    assert(v.size()==3 && v.back()==3);

    v.pop_back();
    assert(v.size()==2 && v.back()==2);

    v.clear();
    assert(v.empty());

    // assign(count, value)
    v.assign(5, 42);
    assert(v.size()==5);
    for (int x: v) assert(x==42);
}

static void test_resize() {
    vector<int> v{1,2,3};
    v.resize(5);           // default-insert 0
    assert(v.size()==5);
    assert(v[3]==0 && v[4]==0);

    v.resize(2);
    assert(v.size()==2);
}

static void test_swap() {
    vector<int> a{1,2,3};
    vector<int> b{9,8};
    a.swap(b);
    assert(a.size()==2 && a[0]==9);
    assert(b.size()==3 && b[2]==3);

    // std::swap should also work if you've injected swap correctly
    mystd::swap(a,b);
    assert(a.size()==3 && a[2]==3);
}

static void test_emplace_back_complex() {
    vector<Emplaceable> v;
    v.emplace_back(7, "hello");
    assert(v.size()==1);
    assert(v[0].x==7 && v[0].s=="hello");
}

int main() {
    test_constructors_and_empty_size();
    test_element_access();
    test_iterators();
    test_capacity_and_reserve_shrink();
    test_modifiers_push_pop_clear_assign();
    test_resize();
    test_swap();
    test_emplace_back_complex();

    std::cout << "All mystd::vector tests passed ✅\n";
    return 0;
}
