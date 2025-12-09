#pragma once
#include <format>
#include <stacktrace>
#include <stdexcept>
#include <string>

namespace utils {

class Exception :
    public std::runtime_error {
public:
    explicit Exception(const std::string& what) :
        std::runtime_error(what),
        trace_(std::stacktrace::current(1)) {}

    const std::stacktrace& stacktrace() const { return trace_; }
private:
    std::stacktrace trace_;
};

template<typename ...Args>
void assert(bool predicate, std::format_string<Args...> msg, Args&& ...args) {
    if (!predicate) {
        throw Exception(std::format(msg, std::forward<Args>(args)...));
    }
}

} // namespace utils

template<>
struct std::formatter<utils::Exception> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return cbegin(ctx);
    }

    auto format(const utils::Exception& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "Exception: {}\n{}", obj.what(), obj.stacktrace());
    }
};
