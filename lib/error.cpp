////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "audio++/error.hpp"

#include <alsa/asoundlib.h>
#include <miniaudio.h>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

class alsa_error_category_ : public std::error_category
{
public:
    ////////////////////
    virtual const char* name() const noexcept override { return "alsa"; }
    virtual std::string message(int ev) const override { return snd_strerror(ev); }
    // TODO: translate to std::errc
};

class mini_error_category_ : public std::error_category
{
public:
    ////////////////////
    virtual const char* name() const noexcept override { return "miniaudio"; }
    virtual std::string message(int ev) const override { return ma_result_description(static_cast<ma_result>(ev)); }
    // TODO: translate to std::errc
};

}

////////////////////////////////////////////////////////////////////////////////
const std::error_category& alsa_error_category()
{
    static alsa_error_category_ instance;
    return instance;
}

const std::error_category& mini_error_category()
{
    static mini_error_category_ instance;
    return instance;
}

////////////////////////////////////////////////////////////////////////////////
}
