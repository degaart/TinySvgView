#include "Util.hpp"
#include <shellapi.h>
#include <fmt/format.h>
#include <stdexcept>

std::string getErrorMessage(DWORD error)
{
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&messageBuffer),
        0,
        nullptr
    );

    std::string errorMessage;
    if (size > 0 && messageBuffer)
    {
        errorMessage = messageBuffer;
        LocalFree(messageBuffer);
    }
    else
    {
        errorMessage = "Unknown error";
    }

    return errorMessage;
}

std::wstring toWideString(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
    if (size_needed == 0)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(fmt::format("Failed to get required buffer size: {}", getErrorMessage(error)));
    }

    std::wstring result(size_needed, 0);
    if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &result[0], size_needed) == 0)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(fmt::format("Failed to convert string: {}", getErrorMessage(error)));
    }

    return result;
}

std::string toMultiByte(const std::wstring& wstr)
{
    if (wstr.empty())
    {
        return std::string();
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (size_needed == 0)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(fmt::format("Failed to get required buffer size: {}", getErrorMessage(error)));
    }

    std::string result(size_needed, 0);
    if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &result[0], size_needed, nullptr, nullptr) == 0)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(fmt::format("Failed to convert string: {}", getErrorMessage(error)));
    }

    return result;
}

std::vector<std::string> splitCommandLine(const std::string& cmdLine)
{
    std::wstring wideCmdLine = toWideString(cmdLine);
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(wideCmdLine.c_str(), &argc);
    if (argv == nullptr)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(fmt::format("Failed to parse command line: {}", getErrorMessage(error)));
    }

    std::vector<std::string> result;
    result.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        result.push_back(toMultiByte(argv[i]));
    }

    LocalFree(argv);
    return result;
}
