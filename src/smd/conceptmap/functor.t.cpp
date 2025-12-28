#include <smd/conceptmap/functor.h>

#include <gtest/gtest.h>

using namespace smd::conceptmap;

template <typename P, const auto &functor = functor_concept_map<P>>
P testP(P p) {
    P x = functor.map(p, [](auto k) { return k; });
    return x;
}

TEST(FunctorTest, TestGTest) { ASSERT_EQ(1, 1); }

constexpr auto opt_int_functor = TransformFunctorMap<std::optional<int>>{};

TEST(FunctorTest, Breathing) {
    Transform<std::optional<int>> impl;
    std::optional<int> oi1{5};
    std::optional<int> oi2 = impl.map(oi1, [](auto k) { return k + 1; });
    ASSERT_EQ(oi2.value(), 6);

    testP<std::optional<int>, opt_int_functor>(oi1);
    testP<std::optional<int>>(oi1);
    auto oi4 = testP(oi1);
    ASSERT_EQ(oi4, oi1);
}

auto my_identity(int t) { return t; }

TEST(FunctorTest, TransformTest) {
    std::optional<int> oi1{5};
    Transform<std::optional<int>> tr;
    std::optional<int> r = tr.map(oi1, my_identity);
    ASSERT_EQ(oi1, r);
}

template <typename P, const auto &functor = functor_concept_map<P>>
auto testP2(P const &p) {
    auto x = functor.map(p, [](auto k) { return static_cast<double>(k); });
    return x;
}

TEST(FunctorTest, TypeTest) {
    std::optional<int> oi1{5};

    auto o4 = testP2(oi1);
    static_assert(std::same_as<decltype(o4), std::optional<double>>);
    std::optional<double> od1{5};
    ASSERT_EQ(o4, od1);
}

TEST(FunctorTest, TransformVectorTest) {
    std::vector<int> v{1, 2, 3, 4, 5};
    RangeTransform<std::vector<int>> tr;
    auto r = tr.map(v, my_identity);
    ASSERT_TRUE(std::ranges::equal(v, r));
}

TEST(FunctorTest, RangeTypeTest) {
    std::vector<int> v{1, 2, 3, 4, 5};

    auto v2 = testP2(v);
    auto r2 = v | std::views::transform(
                      [](auto k) { return static_cast<double>(k); });
    //    static_assert(std::same_as<decltype(v2), decltype(r2)>);
    ASSERT_TRUE(std::ranges::equal(v2, r2));
}

TEST(FunctorTest, BemanOptionalTransformTest) {
    beman::optional::optional<int> oi1{5};
    Transform<beman::optional::optional<int>> tr;
    beman::optional::optional<int> r = tr.map(oi1, my_identity);
    ASSERT_EQ(oi1, r);
}

TEST(FunctorTest, BemanOptionalTypeTest) {
    beman::optional::optional<int> oi1{5};

    auto o4 = testP2(oi1);
    static_assert(
        std::same_as<decltype(o4), beman::optional::optional<double>>);
    beman::optional::optional<double> od1{5};
    ASSERT_EQ(o4, od1);
}
