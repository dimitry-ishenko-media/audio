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
    constexpr span(audio::type type, const void* p, std::size_t count) :
        type_{type}, data_{static_cast<char*>(const_cast<void*>(p))}, size_{count}
    { }

    ////////////////////
    constexpr auto type() const noexcept { return type_; }
    constexpr auto value_size() const noexcept { return audio::size(type_); }

    constexpr auto size() const noexcept { return size_; }
    constexpr auto size_bytes() const noexcept { return size() * value_size(); }

    constexpr auto as_bytes() const noexcept { return std::span{ data_, size_bytes() }; }
    constexpr auto as_bytes() noexcept { return std::span{ data_, size_bytes() }; }

private:
    ////////////////////
    audio::type type_;
    char* data_;
    std::size_t size_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
