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

template <template <typename> typename C, typename T, typename G>
concept FunctorRequirements = requires(C<T> c, G g, T t) {
  std::is_invocable_v<G, T>;
  { c.map(g) } -> std::same_as<C<std::invoke_result<G, T>>>;
};

template <template <typename> typename Impl, typename T, typename G>
  requires FunctorRequirements<Impl, typename Impl<T>::value_type, G>
struct Functor : protected Impl<T> {

  auto map(this auto &&self, auto c, auto g) {
    std::puts("Functor::map");
    return self.map(c, g);
  }

  auto replace(this auto &&self, auto c, auto u) {
    std::puts("Functor::replace");
    return self.map(c, [u]() { return u; });
  }
};

template <typename T>
class Transform {
public:
  using value_type = T;
  auto map(this auto &&/*self*/, auto c, auto g) {
    std::puts("Maybe::map()");
      return c.transform(g);
  }
};

template <typename T>
struct TransformFunctorMap : public Functor<Transform<T>> {
    using Transform<T>::map;
};

} // namespace conceptmap
} // namespace smd

#endif
