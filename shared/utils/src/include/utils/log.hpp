#pragma once
#include "utils/root_path.hpp"

#include <print>
#include <source_location>
#include <filesystem>

namespace utils::log {

enum class Level {
    Debug,
    Info,
    Warn,
    Error
};

template<Level L, typename ...Args>
struct Print {
    Print(std::format_string<Args...> msg, Args &&...args, std::source_location loc = std::source_location::current()) {
        const char* level = "???";
        if constexpr (L == Level::Debug)      level = "Debug";
        else if constexpr (L == Level::Info)  level = "Info";
        else if constexpr (L == Level::Warn)  level = "Warn";
        else if constexpr (L == Level::Error) level = "Error";
        
        const auto path = std::filesystem::relative(loc.file_name(), get_root_path());

        std::println("[{}] {}:{} - {} - {}",
                     level,
                     path.string(),
                     loc.line(),
                     loc.function_name(),
                     std::format(msg, std::forward<Args>(args)...));
    }
};

template<Level L, typename ...Args >
Print(std::format_string<Args...>, Args &&...) -> Print<L, Args...>;

template<typename ...Args>
using debug = Print<Level::Debug, Args...>;

template<typename ...Args>
using info = Print<Level::Info, Args...>;

template<typename ...Args>
using warn = Print<Level::Warn, Args...>;

template<typename ...Args>
using error = Print<Level::Error, Args...>;

} // namespace utils::log
