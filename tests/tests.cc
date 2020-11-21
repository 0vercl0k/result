// Axel '0vercl0k' Souchet - November 20 2020
#include "result.h"

using namespace result;

struct Move_t {
  const size_t Size_ = 0;
  uint8_t *Data_ = nullptr;
  Move_t(const size_t X) : Size_(X), Data_(new uint8_t[Size_]) {
    printf("(%p): ctor allocated %p\n", this, Data_);
  }

  ~Move_t() {
    printf("(%p): dtor releasing %p\n", this, Data_);
    if (Data_ != nullptr) {
      delete[] Data_;
    }
  }

  Move_t(Move_t &&C) noexcept {
    printf("(%p): move ctor\n", this);
    Data_ = std::exchange(C.Data_, nullptr);
  }

  Move_t(const Move_t &B) = delete;
  Move_t &operator=(const Move_t &B) = delete;
  Move_t &operator=(Move_t &&B) = delete;
};

int main() {
  {
    constexpr Result_t<bool, bool> R = Ok(true);
    static_assert(R.Ok(), "Expected to be ok");
    static_assert(R.Unwrap(), "Expected to be true");
  }
  {
    constexpr Result_t<bool, uint32_t> R = Ok(false);
    static_assert(R.Ok(), "Expected to be ok");
    static_assert(!R.Unwrap(), "Expected to be false");
  }
  {
    constexpr Result_t<bool, uint32_t> R = Err(uint32_t(1336));
    static_assert(!R.Ok());
  }
  {
    constexpr Result_t<bool, uint32_t> R = Err(uint32_t(1336));
    static_assert(!R.Ok());
  }
  {
    auto Foo = [](const int A) -> Result_t<void, void> {
      if (A == 1337) {
        return Ok();
      }
      return Err();
    };
    constexpr auto A = Foo(1337);
    static_assert(A.Ok());

    constexpr auto B = Foo(1338);
    static_assert(B.Err());
  }
  {
    auto Foo = [](const int A) -> Result_t<Move_t, int> {
      if (A == 1337) {
        Move_t x(0x1000);
        return Ok(x);
      } else if (A == 1338) {
        Move_t y(0x1000);
        return Ok(y);
      }
      return Err(1337);
    };

    const auto &A = Foo(1337);
    if (A.Err()) {
      std::abort();
    }

    if (A.Ok()) {
      const auto &S = A.Unwrap();
      printf("%p\n", S.Data_);
    }
  }
}