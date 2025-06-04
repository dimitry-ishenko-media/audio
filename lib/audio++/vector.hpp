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
    vector(audio::type type, std::size_t count = 0) : type_{type}, cont_(count * audio::size(type_)) { }

    explicit vector(audio::span span) : type_{span.type()} { append(span); }

    ////////////////////
    constexpr auto type() const noexcept { return type_; }
    constexpr auto value_size() const noexcept { return audio::size(type_); }

    constexpr auto size_bytes() const noexcept { return cont_.size(); }
    constexpr auto size() const noexcept { return size_bytes() / value_size(); }

    constexpr auto data_bytes() const noexcept { return cont_.data(); }
    constexpr auto data_bytes() noexcept { return cont_.data(); }

    ////////////////////
    static constexpr auto npos = static_cast<std::size_t>(-1);

    auto span(std::size_t pos, std::size_t count = npos)
    {
        pos = std::min(pos, size());
        count = std::min(count, size() - pos);
        return audio::span{type_, data_bytes() + pos * value_size(), count};
    }

    void append(audio::span span)
    {
        // TODO: check span.type() == type()
        auto begin = span.data_bytes(), end = begin + span.size_bytes();
        cont_.insert(cont_.end(), begin, end);
    }

    void erase(audio::span span)
    {
        // TODO: check span belongs to this vector
        auto from = cont_.begin() + (span.data_bytes() - cont_.data());
        auto to = from + span.size_bytes();
        cont_.erase(from, to);
    }

private:
    ////////////////////
    audio::type type_;
    std::vector<char> cont_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
