////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "audio++/error.hpp"
#include "audio++/params.hpp"

#include <alsa/asoundlib.h>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto create_params_helper(snd_pcm_t* pcm)
{
    snd_pcm_hw_params_t* params;

    auto ev = snd_pcm_hw_params_malloc(&params);
    if (ev) throw alsa_error{ev, "snd_pcm_hw_params_malloc()"};

    ev = snd_pcm_hw_params_any(pcm, params);
    if (ev) throw alsa_error{ev, "snd_pcm_hw_params_any()"};

    return params;
}

}

////////////////////////////////////////////////////////////////////////////////
params::params(snd_pcm_t* pcm) : pcm_{pcm},
    params_{ create_params_helper(pcm), &snd_pcm_hw_params_free }
{ }

////////////////////////////////////////////////////////////////////////////////
bool params::test(audio::rate rate) const
{
    return !snd_pcm_hw_params_test_rate(pcm_, &*params_, rate, 0);
}

////////////////////////////////////////////////////////////////////////////////
}
