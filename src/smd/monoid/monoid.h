// monoid.h                                                           -*-C++-*-
#ifndef INCLUDED_MONOID
#define INCLUDED_MONOID

//@AUTHOR: Steve Downey (sdowney)

#include <type_traits>
#include <concepts>
#include <algorithm>
#include <ranges>

namespace smd {
namespace monoid {

template<typename T, typename M>
concept MonoidRequirements
= requires(T i) {
    {i.identity()} -> std::same_as<M>;
    }
  ||
  requires(T i, std::ranges::empty_view<M> r1) {
    {i.concat(r1)} -> std::same_as<M>;
    };

template <class Impl> requires MonoidRequirements<Impl, typename Impl::value_type>
struct Monoid : protected Impl {
    auto identity(this auto && self) {
        std::puts("Monoid::identity()");
        return self.concat(std::ranges::empty_view<typename Impl::value_type>{});
    }

   template<typename Range>
   auto concat(this auto&& self, Range r) {
        std::puts("Monoid::concat()");
        return std::ranges::fold_right(r,
                    self.identity(),
                    [&](auto m1, auto m2){return self.op(m1, m2);});
    }

   auto op(this auto&& self, auto a1, auto a2) {
        std::puts("Monoid::op");
        return self.op(a1, a2);
    }
};

template<typename M>
class Plus {
  public:
    using value_type = M;
    auto identity(this auto && /*self*/) -> M{
        std::puts("Plus::identity()");
        return M{0};
    }

    auto op(this auto && /*self*/, auto s1, auto s2) -> M {
        std::puts("Plus::op()");
        return s1 + s2;
    }
};

template<typename M>
struct PlusMonoidMap : public Monoid<Plus<M>> {
    using Plus<M>::identity;
    using Plus<M>::op;
};

class StringMonoid {
  public:
    using value_type = std::string;

    auto op(this auto &&,  auto s1, auto s2) {
        std::puts("StringMonoid::op()");
        return s1 + s2;
    }

   template<typename Range>
   auto concat(this auto&& self, Range r) {
        std::puts("StringMonoid::concat()");
        return std::ranges::fold_right(r, std::string{}, [&](auto m1, auto m2){return self.op(m1, m2);});
    }

};

struct StringMonoidMap : public Monoid<StringMonoid> {
    using StringMonoid::op;
    using StringMonoid::concat;
};


template<class T> auto monoid_concept_map = std::false_type{};

template<>
constexpr inline auto monoid_concept_map<int> = PlusMonoidMap<int>{};

template<>
constexpr inline auto monoid_concept_map<long> = PlusMonoidMap<long>{};

template<>
constexpr inline auto monoid_concept_map<char> = PlusMonoidMap<char>{};

template<>
constexpr inline auto monoid_concept_map<std::string> = StringMonoidMap{};



}  // close package namespace
}  // close smd namespace

#endif
