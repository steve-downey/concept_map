#include <concepts>
#include <string>

namespace N::hidden {
template <typename T>
concept has_eq = requires(T const& v) {
  { eq(v, v) } -> std::same_as<bool>;
};

struct eq_fn {
  template <has_eq T>
  constexpr bool operator()(T const& x,
                            T const& y) const {
    return eq(x, y);
  }
};

template <has_eq T>
constexpr bool ne(T const& x, T const& y) {
  return not eq(x, y);
}

template <typename T>
concept has_ne = requires(T const& v) {
  { ne(v, v) } -> std::same_as<bool>;
};

struct ne_fn {
  template <has_ne T>
  constexpr bool operator()(T const& x,
                            T const& y) const {
    return ne(x, y);
  }
};
} // namespace N::hidden


namespace N {
  inline namespace function_objects {
    inline constexpr hidden::eq_fn eq{};
    inline constexpr hidden::ne_fn ne{};
  }

  template <typename T>
  concept equality_comparable =
    requires (std::remove_reference_t<T> const& t) {
      eq(t, t);
      ne(t, t);
    };
}

struct test {
    std::string id;

    friend bool eq(const test& t1, const test& t2) {
        return t1.id == t2.id;
    }
};

int main() {
    static_assert(N::equality_comparable<test>);

    test t1, t2;
    return N::ne(t1, t2);

}
