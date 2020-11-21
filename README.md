# result

## Overview

Simple, tiny and readable implementation of a `Rust` [std::result](https://doc.rust-lang.org/std/result/) like type for C++.

```c++
```

[C++ 20](https://en.wikipedia.org/wiki/C%2B%2B20) is upon us and the language still doesn't offer a bullet-proof, standardized way to have a function return errors / values (I don't like exceptions). It feels the language has now every pieces required to implement such a type without too much pain and offering good enough guarantees.

The best mechanism I could find was `Rust`'s [std::result].

Here is a list of other source of inspiration as well as similar projects:

- Mozilla's [Result.h](https://searchfox.org/mozilla-central/source/mfbt/Result.h),
- [@tylerreisinger](https://github.com/tylerreisinger)'s [result](https://github.com/tylerreisinger/result),
- [@basicpp17](https://github.com/basicpp17)'s [result17](https://github.com/basicpp17/result17),
- Chromium's []().


## Examples

