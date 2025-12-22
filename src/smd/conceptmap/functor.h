// smd/conceptmap/functor.h -*-C++-*- SPDX-License-Identifier: Apache-2.0 WITH
// LLVM-exception
#ifndef INCLUDED_SMD_CONCEPTMAP_FUNCTOR
#define INCLUDED_SMD_CONCEPTMAP_FUNCTOR

#include <algorithm>
#include <concepts>
#include <ranges>
#include <type_traits>

namespace smd {
namespace conceptmap {

// template <typename Impl, template <typename> typename C, typename T, typename G>
// concept FunctorRequirements = requires(Impl i, C<T> c, T t, G g) {
//     std::is_invocable_v<G, T>;
//     { i.map(c, g) } -> std::same_as<C<std::invoke_result<G, T>>>;
// };

template <template <typename> typename Impl, typename C>
//requires FunctorRequirements<Impl, C, typename Impl<C>::value_type, G>
struct Functor : protected Impl<C> {
    auto map(this auto &&self, C const& c, auto g) {
        std::puts("Functor::map");
        return self.map(c, g);
    }
    auto replace(this auto &&self, C const& c, auto u) {
        std::puts("Functor::replace");
        return self.map(c, [u]() { return u; });
    }
};

template <typename C>
class Transform {
  public:
    using value_type = C::value_type;
    auto map(this auto && /*self*/, C const & c, auto g) {
        std::puts("Transform::map()");
        return c.transform(g);
    }
};

template <typename T>
struct TransformFunctorMap : public Functor<Transform, T> {
    using Transform<T>::map;
};

template <typename C>
auto functor_concept_map = std::false_type{};

template <typename T>
inline constexpr auto functor_concept_map<std::optional<T>> =
    TransformFunctorMap<std::optional<T>>{};

template <typename C>
class RangeTransform {
  public:
    using value_type = C::value_type;
    auto map(this auto && /*self*/, C const& c, auto g) {
        std::puts("RangeTransform::map()");
        return std::views::transform(c, g);
    }
};

template <typename T>
struct RangeTransformFunctorMap : public Functor<RangeTransform, T> {
    using RangeTransform<T>::map;
};

template <template <typename> typename R, typename T>
    requires std::ranges::range<R<T>>
inline constexpr auto functor_concept_map<R<T>> =
    RangeTransformFunctorMap<R<T>>{};

} // namespace conceptmap
} // namespace smd

#endif
