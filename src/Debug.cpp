#include "Debug.hpp"
#include <Windows.h>

void __trace(std::string file, int line, const std::string& func, const std::string& message)
{
    auto slash = file.find_last_of('\\');
    if (slash != std::string::npos)
    {
        file = file.substr(slash + 1);
    }

    auto s = fmt::format("[{}:{}][{}] ", file, line, func);
    s += message;
    s += "\n";
    OutputDebugString(s.c_str());
}
