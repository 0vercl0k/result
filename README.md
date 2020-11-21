# result
![Builds](https://github.com/0vercl0k/result/workflows/Builds/badge.svg)

## Overview

Simple, tiny and readable implementation of a `Rust` [std::result](https://doc.rust-lang.org/std/result/) like type for C++ 20. The implementation uses [std::variant](https://en.cppreference.com/w/cpp/utility/variant) between two user-defined types.

```c++
template <typename OkContent_t, typename ErrContent_t>
class Result {
  using OkType_t = Ok_t<OkContent_t>;
  using ErrType_t = Err_t<ErrContent_t>;
  std::variant<OkType_t, ErrType_t> V_;
};
```

[C++ 20](https://en.wikipedia.org/wiki/C%2B%2B20) is upon us and the language still doesn't offer a bullet-proof, standardized way to have a function return errors / values (I don't like exceptions). It feels the language has now every pieces required to implement such a type without too much pain and offering good enough guarantees.

Here is a list of other source of inspiration as well as similar projects:

- Mozilla's [Result.h](https://searchfox.org/mozilla-central/source/mfbt/Result.h),
- [@tylerreisinger](https://github.com/tylerreisinger)'s [result](https://github.com/tylerreisinger/result),
- [@basicpp17](https://github.com/basicpp17)'s [result17](https://github.com/basicpp17/result17).

## Examples

```c++
namespace fs = std::filesystem
using Buffer_t = std::unique_ptr<uint8_t[]>;

result::Result<Buffer_t, void> ReadFile(const fs::path &Path, size_t &FileSize) {
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
  } catch(...) {
      return Err();
  }

  return Ok(Buffer);
}
```

# Authors

* Axel Souchet ([@0vercl0k](https://twitter.com/0vercl0k))