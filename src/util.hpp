#pragma once

#include "log.hpp"

#include <chrono>
#include <string>

namespace dataheap2
{
inline std::string uuid(const std::string& prefix)
{
    // TODO make something nice... /dev/urandom and such
    return prefix + "." +
           std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

inline auto debug_error_cb(const std::string& prefix)
{
    return [prefix](const auto& message) { log::error("{}: {}", prefix, message); };
}

inline auto debug_success_cb(const std::string& prefix)
{
    return [prefix]() { log::debug("{}", prefix); };
}
}