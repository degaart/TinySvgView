#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <fmt/format.h>

void __trace(std::string file, int line, const std::string& func, const std::string& message);

template <typename... Args>
inline void __trace(const std::string& file, int line, const std::string& func, const std::string& fmt, Args&&... args)
{
    __trace(file, line, func, fmt::format(fmt, std::forward<Args>(args)...));
}

#define trace(fmt, ...) __trace(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)

#endif // DEBUG_HPP
