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

#include <array>
#include <cstddef>
#include <iterator>
#include <memory> // std::to_address
#include <ranges>

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
    constexpr span(audio::type type, const void* p, std::size_t count) : type_{type},
        data_{static_cast<const char*>(p)}, size_{count}
    { }

    template<std::contiguous_iterator Iter>
    constexpr span(Iter iter, std::size_t count) :
        span{type_v<std::iter_value_t<Iter>>, std::to_address(iter), count}
    { }

    template<std::contiguous_iterator Iter>
    constexpr span(Iter from, Iter to) : span{from, to - from} { }

    template<typename T, std::size_t C>
    constexpr span(T (&array)[C]) : span{std::data(array), C} { }

    template<typename T, std::size_t C>
    constexpr span(std::array<T, C>& array) : span{std::data(array), C} { }

    template<std::ranges::contiguous_range R>
    constexpr span(const R& range) : span{std::ranges::data(range), std::ranges::size(range)} { }

    ////////////////////
    constexpr auto type() const noexcept { return type_; }
    constexpr auto value_size() const noexcept { return audio::size(type_); }

    constexpr auto size() const noexcept { return size_; }
    constexpr auto size_bytes() const noexcept { return size() * value_size(); }

    constexpr auto data_bytes() const noexcept { return data_; }
    constexpr auto data_bytes() noexcept { return const_cast<char*>(data_); }

private:
    ////////////////////
    audio::type type_;
    const char* data_;
    std::size_t size_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
