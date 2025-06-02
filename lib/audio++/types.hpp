////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_TYPES_HPP
#define AUDIO_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <cstddef> // std::size_t

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

/**
 * @fn audio::size
 * @brief Get size (in bytes) of an audio::type.
 */
constexpr std::size_t size(audio::type type)
{
    switch (type)
    {
        case  u8: return 1;
        case s16: return 2;
        case s24:
        case s32:
        case f32: return 4;
        default : return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::frames
 * @brief Number of frames.
 */
enum frames : long { };

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
