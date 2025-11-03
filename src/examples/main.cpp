#include <smd/monoid/monoid.h>
#include <iostream>
#include <cassert>
#include <vector>

using namespace smd::monoid;

template <typename P, const auto& monoid = monoid_concept_map<P>>
void testP() {
    auto x = monoid.identity();
    assert(P{} == x);
    auto sum = monoid.op(x, P{1});
    assert(P{1} == sum);
    std::vector<P> v = {1, 2, 3, 4};
    auto           k = monoid.concat(v);
    assert(k == 10);
}

template <typename P, const auto& monoid = monoid_concept_map<P>>
P testP2() {
    auto x  = monoid.identity();
    auto op = monoid.op(x, P{2});
    assert(P{2} == op);
    std::vector<P> v = {1, 2, 3, 4};
    auto           k = monoid.concat(v);
    return k;
}

void test() {
    std::cout << "\ntest int\n";
    testP<int>();
    std::cout << "\ntest long\n";
    testP<long>();
    std::cout << "\ntest char\n";
    testP<char>();

    std::cout << "\ntest int\n";
    int k1 = testP2<int>();
    assert(k1 == 10);

    std::cout << "\ntest int\n";
    int k2 = testP2<int, mult_map<int>>();
    assert(k2 == 24);

    std::cout << "\ntest string\n";
    auto d2 = monoid_concept_map<std::string>;
    auto x2 = d2.identity();
    assert(std::string{} == x2);
    auto sum2 = d2.op(x2, "1");
    assert(std::string{"1"} == sum2);
    std::vector<std::string> vs = {"1", "2", "3", "4"};
    auto                     k3 = d2.concat(vs);
    assert(k3 == std::string{"1234"});
}

int main() { test(); }
