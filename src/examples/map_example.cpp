#include <concepts>
#include <string>

namespace N::hidden {
template <typename T>
concept has_eq = requires(T const& v) {
  { operator==(v, v) } -> std::same_as<bool>;
};

constexpr inline struct partial_eq_default_t {
    constexpr bool eq(has_eq auto const& rhs, has_eq auto const& lhs) const {
        return (rhs == lhs);
    }
    constexpr bool ne(has_eq auto const& rhs, has_eq auto const& lhs) const {
        return (lhs != rhs);
    }
} partial_eq_default;

} // namespace N::hidden

namespace N {

template<class T>
constexpr inline auto partial_eq = hidden::partial_eq_default;

template<class T>
concept partial_equality =
    requires(std::remove_reference_t<T> const& t) {
    {partial_eq<T>.eq(t, t)} -> std::same_as<bool>;
    {partial_eq<T>.ne(t, t)} -> std::same_as<bool>;
};
} // namespace


struct test {
    int id;

    friend auto operator<=>(const test&, const test&) = default;
};

int main() {
    static_assert(N::partial_equality<test>);

    test t1, t2;
    return N::partial_eq<test>.ne(t1, t2);
}

// https://compiler-explorer.com/z/48E7osfxE
