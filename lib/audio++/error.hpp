////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_ERROR_HPP
#define AUDIO_ERROR_HPP

////////////////////////////////////////////////////////////////////////////////
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace audio
{

////////////////////////////////////////////////////////////////////////////////
/**
 * @class audio::error
 * @brief Base class for audio errors.
 */
class error : public std::system_error
{
public:
    ////////////////////
    using std::system_error::system_error;
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @fn audio::alsa_error_category
 * @brief Error category for ALSA errors.
 */
const std::error_category& alsa_error_category();

/**
 * @class audio::alsa_error
 * @brief ALSA error.
 */
class alsa_error : public error
{
public:
    ////////////////////
    explicit alsa_error(int ev) : error{ev, alsa_error_category()} { }
    alsa_error(int ev, const char* msg) : error{ev, alsa_error_category(), msg} { }
    alsa_error(int ev, const std::string& msg) : error{ev, alsa_error_category(), msg} { }
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @fn audio::mini_error_category
 * @brief Error category for miniaudio errors.
 */
const std::error_category& mini_error_category();

/**
 * @class audio::mini_error
 * @brief miniaudio error.
 */
class mini_error : public error
{
public:
    ////////////////////
    explicit mini_error(int ev) : error{ev, mini_error_category()} { }
    mini_error(int ev, const char* msg) : error{ev, mini_error_category(), msg} { }
    mini_error(int ev, const std::string& msg) : error{ev, mini_error_category(), msg} { }
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
