////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "audio++/device.hpp"
#include "audio++/error.hpp"

#include <alsa/asoundlib.h>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto pcm_open_helper(std::string_view name, int stream, int mode)
{
    snd_pcm_t* pcm;
    auto ev = snd_pcm_open(&pcm, name.data(), static_cast<snd_pcm_stream_t>(stream), mode);
    if (ev) throw alsa_error{ev, "snd_pcm_open()"};
    return pcm;
}

}

////////////////////////////////////////////////////////////////////////////////
device::device(std::string name, int stream, int mode) :
    pcm_{ pcm_open_helper(name, stream, mode), &snd_pcm_close }, name_{std::move(name)}, params_{&*pcm_}
{ }

////////////////////////////////////////////////////////////////////////////////
capture::capture(std::string name) : device{std::move(name), SND_PCM_STREAM_CAPTURE, 0} { }
capture::capture(card c) : device{c, SND_PCM_STREAM_CAPTURE, 0} { }

capture::capture(std::string name, nonblock_t) : device{std::move(name), SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK} { }
capture::capture(card c, nonblock_t) : device{c, SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK} { }

////////////////////////////////////////////////////////////////////////////////
playback::playback(std::string name) : device{std::move(name), SND_PCM_STREAM_PLAYBACK, 0} { }
playback::playback(card c) : device{c, SND_PCM_STREAM_PLAYBACK, 0} { }

playback::playback(std::string name, nonblock_t) : device{std::move(name), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK} { }
playback::playback(card c, nonblock_t) : device{c, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK} { }

////////////////////////////////////////////////////////////////////////////////
}
