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
    constexpr span(audio::type type, std::byte* p, frames count) : type_{type}, data_{p}, count_{count} { }

    template<std::contiguous_iterator Iter>
    constexpr span(Iter iter, frames count) : span{type_v<std::iter_value_t<Iter>>, std::to_address(iter), count} { }

    template<std::contiguous_iterator Iter>
    constexpr span(Iter from, Iter to) : span{from, static_cast<frames>(to - from)} { }

    template<typename T, std::size_t C>
    constexpr span(T (&array)[C]) : span{std::data(array), static_cast<frames>(C)} { }

    template<typename T, std::size_t C>
    constexpr span(std::array<T, C>& array) : span{std::data(array), static_cast<frames>(C)} { }

    template<std::ranges::contiguous_range R>
    constexpr span(const R& range) : span{std::ranges::data(range), static_cast<frames>(std::ranges::size(range))} { }

    ////////////////////
    constexpr auto type() const noexcept { return type_; }
    constexpr auto value_size() const noexcept { return audio::size(type_); }

    constexpr auto size() const noexcept { return count_; }
    constexpr std::size_t size_bytes() const noexcept { return size() * value_size(); }

    constexpr auto as_bytes() const noexcept { return data_; }
    constexpr auto as_bytes() noexcept { return data_; }

    template<audio::type A>
    constexpr auto data() const noexcept { return static_cast<const type_t<A>*>(data_); }

    template<audio::type A>
    constexpr auto data() noexcept { return static_cast<type_t<A>*>(data_); }

private:
    ////////////////////
    audio::type type_;
    std::byte* data_;
    audio::frames count_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
