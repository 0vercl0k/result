// Axel '0vercl0k' Souchet - November 20 2020
#include "fmt/format.h"
#include "result.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using Buffer_t = std::unique_ptr<uint8_t[]>;
using namespace result;

struct Move_t {
  const size_t Size_ = 0;
  uint8_t *Data_ = nullptr;
  Move_t(const size_t X) : Size_(X), Data_(new uint8_t[Size_]) {
    fmt::print("{}: ctor allocated {}\n", fmt::ptr(this), fmt::ptr(Data_));
  }

  ~Move_t() {
    fmt::print("{}: dtor releasing {}\n", fmt::ptr(this), fmt::ptr(Data_));
    if (Data_ != nullptr) {
      delete[] Data_;
    }
  }

  Move_t(Move_t &&C) noexcept {
    fmt::print("{}: move ctor\n", fmt::ptr(this));
    Data_ = std::exchange(C.Data_, nullptr);
  }

  Move_t(const Move_t &B) = delete;
  Move_t &operator=(const Move_t &B) = delete;
  Move_t &operator=(Move_t &&B) = delete;
};

result::Result<Buffer_t, void> ReadFile(const fs::path &Path,
                                        size_t &FileSize) {
  std::ifstream File(Path, std::ios::in | std::ios::binary);
  if (!File.is_open()) {
    return Err();
  }

  File.seekg(0, File.end);
  FileSize = File.tellg();
  File.seekg(0, File.beg);

  try {
    auto Buffer = std::make_unique<uint8_t[]>(FileSize);
    File.read((char *)Buffer.get(), FileSize);
    return Ok(std::move(Buffer));
  } catch (...) {
    return Err();
  }
}

int main() {
  {
    size_t Size;
    const auto &Path = fs::current_path().parent_path() / "tests.cc";
    auto R = ReadFile(Path, Size);
    if (R.Err()) {
      std::abort();
    }

    R.Unwrap().release();
  }
  {
    constexpr Result<bool, bool> R = Ok(true);
    static_assert(R.Ok(), "Expected to be ok");
    static_assert(R.Unwrap(), "Ok value expected to be true");
  }
  {
    constexpr Result<bool, uint32_t> R = Ok(false);
    static_assert(R.Ok(), "Expected to be ok");
    static_assert(!R.Unwrap(), "Ok value expected to be false");
  }
  {
    constexpr Result<bool, uint32_t> R = Err(uint32_t(1336));
    static_assert(!R.Ok());
  }
  {
    constexpr Result<bool, uint32_t> R = Err(uint32_t(1336));
    static_assert(!R.Ok());
  }
  {
    auto Foo = [](const int A) -> Result<void, void> {
      if (A == 1337) {
        return Ok();
      }
      return Err();
    };
    constexpr auto A = Foo(1337);
    static_assert(A.Ok(), "Expected to be ok");

    constexpr auto B = Foo(1338);
    static_assert(B.Err(), "Expected to be an error");
  }
  {
    auto Foo = [](const int A) -> Result<Move_t, int> {
      if (A == 1337) {
        return Ok(Move_t(0x1000));
      } else if (A == 1338) {
        Move_t y(0x1000);
        return Ok(std::move(y));
      }
      return Err(1337);
    };

    const auto &A = Foo(1337);
    if (A.Err()) {
      std::abort();
    }

    if (A.Ok()) {
      const auto &S = A.Unwrap();
      fmt::print("{}\n", fmt::ptr(S.Data_));
    }
  }
  {
    auto Foo = [](const int A) -> Result<void, std::string_view> {
      if (A == 1337) {
        return Ok();
      } else if (A == 1338) {
        return Ok();
      }
      return Err(std::string_view("Error message!"));
    };

    constexpr const auto &A = Foo(1336);
    if (A.Ok()) {
      std::abort();
    }

    fmt::print("{}\n", A.UnwrapErr());
  }
  return 0;
}