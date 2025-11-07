#include <concepts>
#include <string>

namespace N::hidden {
template <typename T>
concept has_eq = requires(const T &v) {
    { eq(v, v) } -> std::same_as<bool>;
};

struct eq_fn {
    template <has_eq T>
    constexpr bool operator()(const T &x, const T &y) const {
        return eq(x, y);
    }
};

template <has_eq T>
constexpr bool ne(const T &x, const T &y) {
    return not eq(x, y);
}

template <typename T>
concept has_ne = requires(const T &v) {
    { ne(v, v) } -> std::same_as<bool>;
};

struct ne_fn {
    template <has_ne T>
    constexpr bool operator()(const T &x, const T &y) const {
        return ne(x, y);
    }
};
} // namespace N::hidden

namespace N {
inline namespace function_objects {
inline constexpr hidden::eq_fn eq{};
inline constexpr hidden::ne_fn ne{};
} // namespace function_objects

template <typename T>
concept equality_comparable = requires(const std::remove_reference_t<T> &t) {
    eq(t, t);
    ne(t, t);
};
} // namespace N

struct test {
    std::string id;

    friend bool eq(const test &t1, const test &t2) { return t1.id == t2.id; }
};

int main() {
    static_assert(N::equality_comparable<test>);

    test t1, t2;
    return N::ne(t1, t2);
}
