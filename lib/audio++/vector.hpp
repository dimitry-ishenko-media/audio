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
#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
class vector
{
public:
    ////////////////////
    vector(audio::type type, frames count = frames{0}) : type_{type},
        cont_(count * audio::size(type_))
    { }
    explicit vector(audio::span span) : type_{span.type()} { append(span); }

    ////////////////////
    constexpr auto type() const noexcept { return type_; }
    constexpr auto value_size() const noexcept { return audio::size(type_); }

    constexpr auto size_bytes() const noexcept { return cont_.size(); }
    constexpr auto size() const noexcept { return static_cast<frames>(size_bytes() / value_size()); }

    constexpr auto as_bytes() const noexcept { return cont_.data(); }
    constexpr auto as_bytes() noexcept { return cont_.data(); }

    template<audio::type A> // TODO: check A == type()
    constexpr auto data() const noexcept { return static_cast<const type_t<A>*>(cont_.data()); }

    template<audio::type A> // TODO: check A == type()
    constexpr auto data() noexcept { return static_cast<type_t<A>*>(cont_.data()); }

    ////////////////////
    static constexpr auto npos = static_cast<frames>(-1);

    auto span(frames from, frames count = npos)
    {
        from = std::min(from, size());
        count = std::min(count, frames{size() - from});
        return audio::span{type_, as_bytes() + from * value_size(), count};
    }

    void append(audio::span span)
    {
        // TODO: check span.type() == type()
        auto begin = span.as_bytes(), end = begin + span.size_bytes();
        cont_.insert(cont_.end(), begin, end);
    }

    void erase(audio::span span)
    {
        // TODO: check span belongs to this vector
        auto from = cont_.begin() + (span.as_bytes() - cont_.data());
        auto to = from + span.size_bytes();
        cont_.erase(from, to);
    }

private:
    ////////////////////
    audio::type type_;
    std::vector<std::byte> cont_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
