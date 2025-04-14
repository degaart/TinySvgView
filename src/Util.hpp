#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <windows.h>
#include <vector>

std::string getErrorMessage(DWORD error);
std::wstring toWideString(const std::string& str);
std::string toMultiByte(const std::wstring& wstr);
std::vector<std::string> splitCommandLine(const std::string& cmdLine);

#endif // UTIL_HPP
