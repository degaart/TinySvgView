#include "Debug.hpp"
#include <Windows.h>

#include "TraceWindow.hpp"

static HWND _traceWindow = nullptr;

void __trace(std::string file, int line, const std::string& func, const std::string& message)
{
    auto slash = file.find_last_of('\\');
    if (slash != std::string::npos)
    {
        file = file.substr(slash + 1);
    }

    auto s = fmt::format("[{}:{}][{}] ", file, line, func);
    s += message;

    if (_traceWindow)
    {
        SendMessage(_traceWindow, TraceWindow::UM_TRACE, 0, reinterpret_cast<LPARAM>(s.c_str()));
    }

    s += "\r\n";
    OutputDebugString(s.c_str());
}

void setTraceWindow(HWND hwnd)
{
    _traceWindow = hwnd;
}

