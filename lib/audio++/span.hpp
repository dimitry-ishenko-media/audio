////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_SPAN_HPP
#define AUDIO_SPAN_HPP

////////////////////////////////////////////////////////////////////////////////
#include "audio++/types.hpp"

#include <cstddef>
#include <span>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
/**
 * @class audio::span
 * @brief Non-owning contiguous sequence of audio samples.
 */
class span
{
public:
    ////////////////////
    constexpr span(audio::format fmt, const void* data, std::size_t count) :
        fmt_{fmt}, data_{static_cast<char*>(const_cast<void*>(data))}, size_{count}
    { }

    ////////////////////
    constexpr auto&& format() const noexcept { return fmt_; }
    constexpr auto frame_size() const noexcept { return fmt_.size(); }

    constexpr auto size() const noexcept { return size_; }
    constexpr auto size_bytes() const noexcept { return size() * frame_size(); }

    constexpr auto as_bytes() const noexcept { return std::span{ data_, size_bytes() }; }
    constexpr auto as_bytes() noexcept { return std::span{ data_, size_bytes() }; }

private:
    ////////////////////
    audio::format fmt_;
    char* data_;
    std::size_t size_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
