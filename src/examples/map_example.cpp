#include <concepts>
#include <string>

namespace N::hidden {
template <typename T>
concept has_eq = requires(const T &v) {
    { operator==(v, v) } -> std::same_as<bool>;
};

inline constexpr struct partial_eq_default_t {
    constexpr bool eq(const has_eq auto &rhs, const has_eq auto &lhs) const {
        return (rhs == lhs);
    }
    constexpr bool ne(const has_eq auto &rhs, const has_eq auto &lhs) const {
        return (lhs != rhs);
    }
} partial_eq_default;

} // namespace N::hidden

namespace N {

template <class T>
inline constexpr auto partial_eq = hidden::partial_eq_default;

template <class T>
concept partial_equality = requires(const std::remove_reference_t<T> &t) {
    { partial_eq<T>.eq(t, t) } -> std::same_as<bool>;
    { partial_eq<T>.ne(t, t) } -> std::same_as<bool>;
};
} // namespace N

struct test {
    int id{};

    friend auto operator<=>(const test &, const test &) = default;
};

int main() {
    static_assert(N::partial_equality<test>);

    test t1, t2;
    return N::partial_eq<test>.ne(t1, t2);
}

// https://compiler-explorer.com/z/48E7osfxE
