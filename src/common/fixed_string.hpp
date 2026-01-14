#pragma once

#include <cstddef>

#include <array>

namespace ctr {

template <typename CharT, size_t N>
struct fixed_string {
  std::array<CharT, N> data;

  constexpr CharT operator[](size_t i) const noexcept { return data[i]; }

  constexpr fixed_string(const CharT (&str)[N]) noexcept
  {
    for (size_t i = 0; i < N; ++i)
      data[i] = str[i];
  }

  constexpr size_t size() const noexcept { return N; }
};

template <typename CharT, size_t N>
fixed_string(const CharT (&)[N]) -> fixed_string<CharT, N>;

} // namespace ctr
