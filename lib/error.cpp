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

    virtual std::error_condition default_error_condition(int ev) const noexcept override
    {
        switch (-ev) // ALSA errors are negative
        {
            case SND_ERROR_INCOMPATIBLE_VERSION: return std::errc::invalid_argument;
            case SND_ERROR_ALISP_NIL: return std::errc::function_not_supported;

            default: return std::system_category().default_error_condition(-ev);
        }
    }
};

class mini_error_category_ : public std::error_category
{
public:
    ////////////////////
    virtual const char* name() const noexcept override { return "miniaudio"; }
    virtual std::string message(int ev) const override { return ma_result_description(static_cast<ma_result>(ev)); }

    virtual std::error_condition default_error_condition(int ev) const noexcept override
    {
        switch (ev)
        {
            case MA_ACCESS_DENIED:          return std::errc::permission_denied;
            case MA_ADDRESS_FAMILY_NOT_SUPPORTED: return std::errc::address_family_not_supported;
            case MA_ALREADY_CONNECTED:      return std::errc::already_connected;
            case MA_ALREADY_EXISTS:         return std::errc::file_exists;
            case MA_ALREADY_IN_USE:         return std::errc::address_in_use;
            case MA_API_NOT_FOUND:          return std::errc::invalid_argument;
            case MA_AT_END:                 return std::errc::state_not_recoverable;
            case MA_BACKEND_NOT_ENABLED:    return std::errc::invalid_argument;
            case MA_BAD_ADDRESS:            return std::errc::bad_address;
            case MA_BAD_MESSAGE:            return std::errc::bad_message;
            case MA_BAD_PIPE:               return std::errc::broken_pipe;
            case MA_BAD_PROTOCOL:           return std::errc::invalid_argument;
            case MA_BAD_SEEK:               return std::errc::invalid_seek;
            case MA_BUSY:                   return std::errc::device_or_resource_busy;
            case MA_CANCELLED:              return std::errc::operation_canceled;
            case MA_CONNECTION_REFUSED:     return std::errc::connection_refused;
            case MA_CONNECTION_RESET:       return std::errc::connection_reset;
            case MA_CRC_MISMATCH:           return std::errc::invalid_argument;
            case MA_DEADLOCK:               return std::errc::resource_deadlock_would_occur;
            case MA_DEVICE_ALREADY_INITIALIZED: return std::errc::operation_in_progress;
            case MA_DEVICE_NOT_INITIALIZED: return std::errc::operation_not_permitted;
            case MA_DEVICE_NOT_STARTED:     return std::errc::operation_in_progress;
            case MA_DEVICE_NOT_STOPPED:     return std::errc::operation_in_progress;
            case MA_DEVICE_TYPE_NOT_SUPPORTED: return std::errc::not_supported;
            case MA_DIRECTORY_NOT_EMPTY:    return std::errc::directory_not_empty;
            case MA_DOES_NOT_EXIST:         return std::errc::no_such_device;
            case MA_ERROR:                  return std::errc::io_error;
            case MA_FAILED_TO_INIT_BACKEND: return std::errc::io_error;
            case MA_FAILED_TO_OPEN_BACKEND_DEVICE: return std::errc::io_error;
            case MA_FAILED_TO_START_BACKEND_DEVICE: return std::errc::io_error;
            case MA_FAILED_TO_STOP_BACKEND_DEVICE: return std::errc::io_error;
            case MA_FORMAT_NOT_SUPPORTED:   return std::errc::not_supported;
            case MA_INTERRUPT:              return std::errc::interrupted;
            case MA_INVALID_ARGS:           return std::errc::invalid_argument;
            case MA_INVALID_DATA:           return std::errc::invalid_argument;
            case MA_INVALID_DEVICE_CONFIG:  return std::errc::invalid_argument;
            case MA_INVALID_FILE:           return std::errc::invalid_argument;
            case MA_INVALID_OPERATION:      return std::errc::operation_not_supported;
            case MA_IN_PROGRESS:            return std::errc::operation_in_progress;
            case MA_IO_ERROR:               return std::errc::io_error;
            case MA_IS_DIRECTORY:           return std::errc::is_a_directory;
            case MA_LOOP:                   return std::errc::io_error;
            case MA_MEMORY_ALREADY_MAPPED:  return std::errc::operation_in_progress;
            case MA_NAME_TOO_LONG:          return std::errc::filename_too_long;
            case MA_NOT_CONNECTED:          return std::errc::not_connected;
            case MA_NOT_DIRECTORY:          return std::errc::not_a_directory;
            case MA_NOT_IMPLEMENTED:        return std::errc::function_not_supported;
            case MA_NOT_SOCKET:             return std::errc::not_a_socket;
            case MA_NOT_UNIQUE:             return std::errc::invalid_argument;
            case MA_NO_ADDRESS:             return std::errc::destination_address_required;
            case MA_NO_BACKEND:             return std::errc::no_such_device;
            case MA_NO_DATA_AVAILABLE:      return std::errc::no_message_available;
            case MA_NO_DEVICE:              return std::errc::no_such_device;
            case MA_NO_HOST:                return std::errc::host_unreachable;
            case MA_NO_MESSAGE:             return std::errc::no_message;
            case MA_NO_NETWORK:             return std::errc::network_unreachable;
            case MA_NO_SPACE:               return std::errc::no_space_on_device;
            case MA_OUT_OF_MEMORY:          return std::errc::not_enough_memory;
            case MA_OUT_OF_RANGE:           return std::errc::result_out_of_range;
            case MA_PATH_TOO_LONG:          return std::errc::filename_too_long;
            case MA_PROTOCOL_FAMILY_NOT_SUPPORTED: return std::errc::protocol_not_supported;
            case MA_PROTOCOL_NOT_SUPPORTED: return std::errc::protocol_not_supported;
            case MA_PROTOCOL_UNAVAILABLE:   return std::errc::protocol_error;
            case MA_SHARE_MODE_NOT_SUPPORTED: return std::errc::operation_not_supported;
            case MA_SOCKET_NOT_SUPPORTED:   return std::errc::not_supported;
            case MA_TIMEOUT:                return std::errc::timed_out;
            case MA_TOO_BIG:                return std::errc::file_too_large;
            case MA_TOO_MANY_LINKS:         return std::errc::too_many_links;
            case MA_TOO_MANY_OPEN_FILES:    return std::errc::too_many_files_open;
            case MA_UNAVAILABLE:            return std::errc::resource_unavailable_try_again;

            default: return {ev, mini_error_category()}; // this shouldn't happen, but...
        }
    }
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
