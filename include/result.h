// Axel '0vercl0k' Souchet - November 20 2020
#pragma once
#include <cstdint>
#include <variant>
#include <Windows.h>
#include <optional>

namespace result {
    template<typename OkContent_t> struct Ok_t {
        OkContent_t Ok;
        constexpr Ok_t(OkContent_t&& Content) : Ok(std::move(Content)) {}
        /// <summary>
        /// Result_t needs to be able to move an Ok_t into the variant.
        /// </summary>
        /// <param name=""></param>
        constexpr Ok_t(Ok_t&&) = default;

        Ok_t(const Ok_t&) = delete;
        Ok_t& operator=(const Ok_t&) = delete;
        Ok_t& operator=(Ok_t&&) = delete;
    };
    template<> struct Ok_t<void> {};

    template<typename ErrorContent_t> struct Err_t {
        ErrorContent_t Err;
        constexpr Err_t(ErrorContent_t&& ErrorContent) : Err(std::move(ErrorContent)) {};
        constexpr Err_t(Err_t&&) = default;

        Err_t(const Err_t&) = delete;
        Err_t& operator=(const Err_t&) = delete;
        Err_t& operator=(Err_t&&) = delete;
    };

    template<> struct Err_t<void> {};

    template<typename OkContent_t, typename ErrorContent_t>
    struct [[nodiscard("Bleh")]] Result_t {
        using OkType_t = Ok_t<OkContent_t>;
        using ErrorType_t = Err_t<ErrorContent_t>;
        std::variant<OkType_t, ErrorType_t> V_;

        constexpr Result_t(OkType_t&& Ok) : V_(std::move(Ok)) {}
        constexpr Result_t(ErrorType_t&& Error) : V_(std::move(Error)) {}

        Result_t(const Result_t&) = delete;
        Result_t& operator=(const Result_t&) = delete;
        Result_t(Result_t&&) = delete;
        Result_t& operator=(Result_t&&) = delete;

        constexpr bool Err() const {
            return std::holds_alternative<ErrorType_t>(V_);
        }

        constexpr bool Ok() const {
            return !Err();
        }

        constexpr const auto& Unwrap() const {
            return std::get<OkType_t>(V_).Ok;
        }
    };
}

template<typename ErrContent_t>
[[nodiscard("This return value should not be discarded; mistake?")]] constexpr auto Err(ErrContent_t&& ErrContent) {
    return result::Err_t(std::move(ErrContent));
}

[[nodiscard("This return value should not be discarded; mistake?")]] constexpr auto Err() {
    return result::Err_t<void>();
}

template<typename OkContent_t>
[[nodiscard("This return value should not be discarded; mistake ?")]] constexpr auto Ok(OkContent_t&& OkContent) {
    return result::Ok_t(std::move(OkContent));
}

constexpr auto Ok() {
    return result::Ok_t<void>();
}