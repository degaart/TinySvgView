#include "Window.hpp"

#include <cassert>

#include "Util.hpp"
#include <fmt/format.h>
#include <stdexcept>

#include "Debug.hpp"

Window::Window(HINSTANCE hInstance)
    : _hwnd(nullptr),
      _hInstance(hInstance)
{
    strcpy(_magic, MAGIC);
}

Window::Window(Window&& other) noexcept
    : _hwnd(nullptr),
      _hInstance(nullptr)
{
    moveFrom(other);
}

Window& Window::operator=(Window&& other) noexcept
{
    if (this != &other)
    {
        if (_hwnd)
        {
            DestroyWindow(_hwnd);
        }
        moveFrom(other);
    }
    return *this;
}

void Window::moveFrom(Window& other) noexcept
{
    _hwnd = other._hwnd;
    _hInstance = other._hInstance; 
    other._hwnd = nullptr;
    other._hInstance = nullptr;
}

Window::~Window()
{
}

void Window::registerWindowClass(
    HINSTANCE hInstance,
    unsigned int style,
    HICON icon,
    HCURSOR cursor,
    HBRUSH background,
    const char* menuName,
    const std::string& className,
    HICON iconSm
)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = style;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className.c_str();
    wc.hCursor = cursor;
    wc.hbrBackground = background;
    wc.lpszMenuName = menuName;
    wc.hIcon = icon;
    wc.hIconSm = iconSm;

    if (!RegisterClassEx(&wc))
    {
        throw std::runtime_error("Failed to register window class");
    }
}

void Window::create(
    const std::string& className,
    const std::string& title,
    DWORD style,
    int x,
    int y,
    int width,
    int height,
    HWND parent,
    HMENU menu
)
{
    _hwnd = CreateWindowEx(
        0,
        className.c_str(),
        title.c_str(),
        style,
        x,
        y,
        width,
        height,
        parent,
        menu,
        _hInstance,
        this
    );

    if (_hwnd == nullptr)
    {
        throw std::runtime_error(fmt::format("Failed to create window: {}", getErrorMessage(GetLastError())));
    }
}

void Window::show(int nCmdShow)
{
    ShowWindow(_hwnd, nCmdShow);
    UpdateWindow(_hwnd);
}

void Window::update()
{
    UpdateWindow(_hwnd);
}

LRESULT CALLBACK Window::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pThis = nullptr;
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->_hwnd = hwnd;
    }
    else if (uMsg == WM_NCDESTROY)
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        if (pThis)
        {
            pThis->_hwnd = nullptr;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    else
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (pThis)
        {
            assert(!strcmp(pThis->_magic, MAGIC));
        }
    }
    
    if (pThis)
    {
        auto ret = pThis->handleMessage(uMsg, wParam, lParam);
        return ret;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

