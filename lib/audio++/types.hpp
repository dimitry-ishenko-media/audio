////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_TYPES_HPP
#define AUDIO_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <cstddef>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::chans
 * @brief Count of audio channels.
 */
enum chans : int { mono = 1, stereo = 2 };

namespace literals
{
constexpr auto operator"" _ch (unsigned long long n) noexcept { return static_cast<chans>(n); }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::rate
 * @brief Audio sample rate.
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
 * @brief Type of audio samples.
 */
enum type : int
{
    u8, s16, s24, s32, f32,
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @fn audio::bits
 * @brief Get count of bits for an audio::type value.
 */
constexpr std::size_t bits(audio::type type) noexcept
{
    switch (type)
    {
    case u8 : return 8;
    case s16: return 16;
    case s24: return 24;
    case s32:
    case f32: return 32;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * @fn audio::size
 * @brief Get size (in bytes) of an audio::type value.
 */
constexpr std::size_t size(audio::type type) noexcept
{
    switch (type)
    {
        case u8 : return 1;
        case s16: return 2;
        case s24:
        case s32:
        case f32: return 4;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @struct audio::format
 * @brief Audio format (count of channels, sample rate and sample type).
 */
struct format
{
    audio::chans chans;
    audio::rate rate;
    audio::type type;

    ////////////////////
    constexpr std::size_t size() const noexcept { return chans * audio::size(type); }

    constexpr bool operator==(const format&) const noexcept = default;
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum audio::card
 * @brief Audio card number.
 */
enum card : int { };

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @var audio::nonblock
 * @brief Tag for non-blocking mode.
 */
struct nonblock_t { explicit nonblock_t() = default; };
inline constexpr nonblock_t nonblock { };

////////////////////////////////////////////////////////////////////////////////
#endif
