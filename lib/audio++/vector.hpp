////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_VECTOR_HPP
#define AUDIO_VECTOR_HPP

////////////////////////////////////////////////////////////////////////////////
#include "audio++/span.hpp"
#include "audio++/types.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
class vector
{
public:
    ////////////////////
    explicit vector(audio::format fmt, std::size_t count = 0) : fmt_{fmt}, data_(count * fmt_.size()) { }
    explicit vector(audio::span span) : fmt_{span.format()} { append(span); }

    ////////////////////
    constexpr auto&& format() const noexcept { return fmt_; }
    constexpr auto frame_size() const noexcept { return fmt_.size(); }

    constexpr auto size_bytes() const noexcept { return data_.size(); }
    constexpr auto size() const noexcept { return size_bytes() / frame_size(); }

    constexpr auto&& as_bytes() const noexcept { return data_; }
    constexpr auto&& as_bytes() noexcept { return data_; }

    ////////////////////
    static constexpr auto npos = static_cast<std::size_t>(-1);

    auto span(std::size_t pos, std::size_t count = npos)
    {
        pos = std::min(pos, size());
        count = std::min(count, size() - pos);

        return audio::span{fmt_, data_.data() + pos * frame_size(), count};
    }

    void append(audio::span span)
    {
        assert(span.format() == format());

        auto bytes = span.as_bytes();
        data_.insert(data_.end(), bytes.begin(), bytes.end());
    }

private:
    ////////////////////
    audio::format fmt_;
    std::vector<char> data_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
