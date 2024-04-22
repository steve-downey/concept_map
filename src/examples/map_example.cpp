#include <concepts>
#include <string>

namespace N::hidden {
template <typename T>
concept has_eq = requires(T const& v) {
  { eq(v, v) } -> std::same_as<bool>;
};

constexpr inline struct {
    constexpr bool eq_(has_eq auto const& rhs, has_eq auto const& lhs) const {
        return eq(rhs, lhs);
    }
    constexpr bool ne_(has_eq auto const& rhs, has_eq auto const& lhs) const {
        return not eq(lhs, rhs);
    }
} partial_eq_default;

} // namespace N::hidden

namespace N {

template<class T>
constexpr inline auto partial_eq = hidden::partial_eq_default;

template<class T>
concept partial_equality =
    requires(std::remove_reference_t<T> const& t) {
    {partial_eq<T>.eq_(t, t)} -> std::same_as<bool>;
    {partial_eq<T>.ne_(t, t)} -> std::same_as<bool>;
};
} // namespace


struct test {
    std::string id;

    friend bool eq(const test& t1, const test& t2) {
        return t1.id == t2.id;
    }
};

int main() {
    static_assert(N::partial_equality<test>);

    test t1, t2;
    return N::partial_eq<test>.eq_(t1, t2);

}
