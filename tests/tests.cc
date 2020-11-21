// Axel '0vercl0k' Souchet - November 20 2020
#include "result.h"

using namespace result;

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
		struct Big_t {
			const size_t Size_ = 0;
			uint8_t* Data_ = nullptr;
			Big_t(const size_t X) : Size_(X), Data_(new uint8_t[Size_]) {
				printf("Big_t::Big_t(%p): ctor allocated %p\n", this, Data_);
			}

			~Big_t() {
				printf("Big_t::~Big_t(%p): dtor releasing %p\n", this, Data_);
				if (Data_ != nullptr) {
					delete[] Data_;
				}
			}

			Big_t(Big_t&& C) noexcept {
				printf("Big_t::Big_t(%p): move ctor\n", this);
				Data_ = std::exchange(C.Data_, nullptr);
			}

			Big_t(const Big_t& B) = delete;
			Big_t& operator=(const Big_t& B) = delete;
			Big_t& operator=(Big_t&& B) = delete;
		};

		auto Foo = [](const int A) -> Result_t<Big_t, DWORD> {
			if (A == 1337) {
				Big_t x(0x1000);
				return Ok(x);
			}
			Big_t y(0x1000);
			return Ok(y);
		};

		const auto& A = Foo(1337);
		printf("sizeof(A) %zd, sizeof(Big_t) %zd\n", sizeof(A), sizeof(Big_t));
		__debugbreak();
		if (A.Ok()) {
			const auto& S = A.Unwrap();
			printf("%p\n", S.Data_);
		}
	}
}