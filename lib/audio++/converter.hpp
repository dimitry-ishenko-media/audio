////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_CONVERTER_HPP
#define AUDIO_CONVERTER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "audio++/span.hpp"
#include "audio++/types.hpp"
#include "audio++/vector.hpp"

#include <memory>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
struct converter_options
{
    audio::format in, out;
};

class converter
{
public:
    ////////////////////
    explicit converter(const converter_options&);

    vector process(span);

private:
    ////////////////////
    // ma_converter is a typedef to an anonymous struct,
    // so we can't forward-declare it and have to use void*
    std::unique_ptr<void, void (*)(void*)> converter_;

    audio::format fmt_in_, fmt_out_;
    audio::vector store_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
