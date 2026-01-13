#include <cstddef>
#include <array>

template <typename CharT, size_t N> struct fixed_string {

    std::array<CharT, N> data;

    constexpr CharT operator[](size_t i) const noexcept { return data[i]; }
    constexpr size_t size() const noexcept { return N; }
};

template <typename CharT, size_t N> fixed_string(const CharT (&)[N]) -> fixed_string<CharT, N>;
