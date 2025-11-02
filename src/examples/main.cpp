#include <smd/monoid/monoid.h>
#include <iostream>
#include <cassert>
#include <vector>

using namespace smd::monoid;

template<typename P>
void testP()
{
    auto d1 = monoid_concept_map<P>;
    auto x = d1.identity();
    assert(P{} == x);
    auto sum = d1.op(x, P{1});
    assert(P{1} == sum);
    std::vector<P> v = {1,2,3,4};
    auto k = d1.concat(v);
    assert(k == 10);
}

void test()
{
    std::cout << "\ntest int\n";
    testP<int>();
    std::cout << "\ntest long\n";
    testP<long>();
    std::cout << "\ntest char\n";
    testP<char>();

    std::cout << "\ntest string\n";
    auto d2 = monoid_concept_map<std::string>;
    auto x2 = d2.identity();
    assert(std::string{} == x2);
    auto sum2 = d2.op(x2, "1");
    assert(std::string{"1"} == sum2);
    std::vector<std::string> vs = {"1","2","3","4"};
    auto k2 = d2.concat(vs);
    assert(k2 == std::string{"1234"});
}


int main()
{
    test();
}
