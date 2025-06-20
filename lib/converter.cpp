////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "audio++/converter.hpp"
#include "audio++/error.hpp"
#include "internal.hpp" // audio::to_ma_format

#include <cassert>
#include <miniaudio.h>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto converter_create_helper(const converter_options& options)
{
    auto config = ma_data_converter_config_init(
        to_ma_format(options.in.type),
        to_ma_format(options.out.type),
        options.in.chans,
        options.out.chans,
        options.in.rate,
        options.out.rate
    );

    auto converter = new ma_data_converter;
    if (auto ev = ma_data_converter_init(&config, nullptr, converter); ev != MA_SUCCESS)
    {
        delete converter;
        throw audio::mini_error{ev, "ma_data_converter_init()"};
    }
    else return converter;
}

void converter_destroy_helper(void* p)
{
    auto converter = static_cast<ma_data_converter*>(p);
    ma_data_converter_uninit(converter, nullptr);
    delete converter;
}

}

////////////////////////////////////////////////////////////////////////////////
converter::converter(const converter_options& options) :
    converter_{ converter_create_helper(options), &converter_destroy_helper },
    fmt_in_{options.in}, fmt_out_{options.out}, store_{options.in}
{ }

////////////////////////////////////////////////////////////////////////////////
vector converter::process(audio::span data_in)
{
    assert(data_in.format() == fmt_in_);

    // append to unprocessed data from the previous call
    store_.append(data_in);

    auto converter = static_cast<ma_data_converter*>(converter_.get());
    ma_uint64 count_in = store_.size(), count_out;

    auto ev = ma_data_converter_get_expected_output_frame_count(converter, count_in, &count_out);
    if (ev != MA_SUCCESS) throw mini_error{ev, "ma_data_converter_get_expected_output_frame_count()"};

    audio::vector data_out{fmt_out_, count_out};
    
    ev = ma_data_converter_process_pcm_frames(converter,
        store_.as_bytes().data(), &count_in,
        data_out.as_bytes().data(), &count_out
    );
    if (ev != MA_SUCCESS) throw mini_error{ev, "ma_converter_process_pcm_frames()"};

    // store unprocessed data
    store_ = vector{store_.span(count_in, store_.npos)};

    return data_out;
}

////////////////////////////////////////////////////////////////////////////////
}
