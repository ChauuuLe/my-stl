#include "../containers/vector/vector.hpp"
#include <iostream>
#include <cassert>

void test_push_back() {
    mystd::vector<int> vec;
    vec.push_back(5);
    vec.push_back(10);
    assert(vec.size() == 2);
    assert(vec[0] == 5);
    assert(vec[1] == 10);
}

void test_resize() {
    mystd::vector<int> vec(2, 4);
    vec.resize(5, 7);
    assert(vec.size() == 5);
    assert(vec[0] == 4 && vec[4] == 7);
}

int main() {
    test_push_back();
    test_resize();
    std::cout << "All tests passed.\n";
    return 0;
}
