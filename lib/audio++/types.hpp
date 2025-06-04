////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_TYPES_HPP
#define AUDIO_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include <stdexcept>
#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::channels
 * @brief Number of channels.
 */
enum channels : int { mono = 1, stereo = 2 };

namespace literals
{
constexpr auto operator"" _ch (unsigned long long n) noexcept { return static_cast<channels>(n); }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::rate
 * @brief Sample rate.
 */
enum rate : int { };

namespace literals
{
constexpr auto operator"" _hz (unsigned long long n) noexcept { return static_cast<rate>(n); }
constexpr auto operator"" _khz(unsigned long long n) noexcept { return static_cast<rate>(n * 1000); }
constexpr auto operator"" _khz(long double n) noexcept { return static_cast<rate>(n * 1000); }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::type
 * @brief Sample type.
 */
enum type : int
{
    u8, s16, s24, s32, f32,
};

namespace internal
{
template<typename T>
constexpr auto always_false = false;

template<typename T>
constexpr auto T2type()
{
    if constexpr (std::is_same_v<T, std::uint8_t>) return u8;
    else if constexpr (std::is_same_v<T, std::int16_t>) return s16;
    else if constexpr (std::is_same_v<T, std::int32_t>) return s32;
    else if constexpr (std::is_same_v<T, float>) return f32;
    else static_assert(always_false<T>, "Bad sample type");
}

template<audio::type> struct type2T { };

template<> struct type2T<u8 > { using type = std::uint8_t; };
template<> struct type2T<s16> { using type = std::int16_t; };
template<> struct type2T<s24> { using type = std::int32_t; };
template<> struct type2T<s32> { using type = std::int32_t; };
template<> struct type2T<f32> { using type = float; };
}

/**
 * @var audio::type_v
 * @brief Get audio::type value for a given type T.
 */
template<typename T>
constexpr auto type_v = internal::T2type<T>();

/**
 * @typedef type_t
 * @brief Get type T for a given audio::type value.
 */
template<audio::type A>
using type_t = typename internal::type2T<A>::type;

/**
 * @fn audio::size
 * @brief Get size (in bytes) of an audio::type value.
 */
constexpr std::size_t size(audio::type type)
{
    switch (type)
    {
        case u8 : return sizeof(type_t<u8 >);
        case s16: return sizeof(type_t<s16>);
        case s24: return sizeof(type_t<s24>);
        case s32: return sizeof(type_t<s32>);
        case f32: return sizeof(type_t<f32>);
        default : throw std::invalid_argument{"Bad sample type"};
    }
}

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
