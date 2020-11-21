// Axel '0vercl0k' Souchet - November 20 2020
#pragma once
#include <cstdint>
#include <variant>

namespace result {
template <typename OkContent_t> struct Ok_t {
  OkContent_t Ok;
  constexpr Ok_t(OkContent_t &&Content) : Ok(std::move(Content)) {}
  constexpr Ok_t(Ok_t &&) = default;

  Ok_t(const Ok_t &) = delete;
  Ok_t &operator=(const Ok_t &) = delete;
  Ok_t &operator=(Ok_t &&) = delete;
};
template <> struct Ok_t<void> {};

template <typename ErrorContent_t> struct Err_t {
  ErrorContent_t Err;
  constexpr Err_t(ErrorContent_t &&ErrorContent)
      : Err(std::move(ErrorContent)){};
  constexpr Err_t(Err_t &&) = default;

  Err_t(const Err_t &) = delete;
  Err_t &operator=(const Err_t &) = delete;
  Err_t &operator=(Err_t &&) = delete;
};
template <> struct Err_t<void> {};

template <typename OkContent_t, typename ErrContent_t>
class [[nodiscard("This should not be discarded")]] Result {
  using OkType_t = Ok_t<OkContent_t>;
  using ErrType_t = Err_t<ErrContent_t>;
  std::variant<OkType_t, ErrType_t> V_;

public:
  constexpr Result(OkType_t && Ok) : V_(std::move(Ok)) {}
  constexpr Result(ErrType_t && Error) : V_(std::move(Error)) {}

  Result(const Result &) = delete;
  Result &operator=(const Result &) = delete;
  Result(Result &&) = delete;
  Result &operator=(Result &&) = delete;

  [[nodiscard("This should not be discarded")]] constexpr bool Err()
      const noexcept {
    return V_.index() == 1;
  }

  [[nodiscard("This should not be discarded")]] constexpr bool Ok()
      const noexcept {
    return !Err();
  }

  [[nodiscard("This should not be discarded")]] constexpr auto &
  Unwrap() noexcept {
    assert(V_.index() == 0);
    return std::get<OkType_t>(V_).Ok;
  }

  [[nodiscard("This should not be discarded")]] constexpr auto &Unwrap()
      const noexcept {
    assert(V_.index() == 0);
    return std::get<OkType_t>(V_).Ok;
  }

  [[nodiscard("This should not be discarded")]] constexpr auto &
  UnwrapErr() noexcept {
    assert(V_.index() == 1);
    return std::get<ErrType_t>(V_).Ok;
  }

  [[nodiscard("This should not be discarded")]] constexpr auto &UnwrapErr()
      const noexcept {
    assert(V_.index() == 1);
    return std::get<ErrType_t>(V_).Ok;
  }
};
} // namespace result

template <typename ErrContent_t>
[[nodiscard("This should not be discarded")]] constexpr auto
Err(ErrContent_t &&ErrContent) {
  return result::Err_t(std::move(ErrContent));
}

[[nodiscard("This should not be discarded")]] constexpr auto Err() {
  return result::Err_t<void>();
}

template <typename OkContent_t>
[[nodiscard("This should not be discarded")]] constexpr auto
Ok(OkContent_t &&OkContent) {
  return result::Ok_t(std::move(OkContent));
}

[[nodiscard("This should not be discarded")]] constexpr auto Ok() {
  return result::Ok_t<void>();
}