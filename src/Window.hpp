#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <windows.h>
#include <string>
#include <functional>

class Window
{
public:
    Window(HINSTANCE hInstance);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;
    virtual ~Window();

    void create(
        const std::string& className,
        const std::string& title,
        DWORD style,
        int x,
        int y,
        int width,
        int height,
        HWND parent = nullptr,
        HMENU menu = nullptr
    );
    void show(int nCmdShow = SW_SHOW);
    void update();
    HWND getHandle() const { return _hwnd; }
    static void registerWindowClass(
        HINSTANCE hInstance,
        unsigned int style,
        HICON icon,
        HCURSOR cursor,
        HBRUSH background,
        const char* menuName,
        const std::string& className,
        HICON iconSm
    );

protected:
    HWND _hwnd;
    HINSTANCE _hInstance;
    char _magic[64];

    static constexpr auto MAGIC = "SKIBIDI";

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    void moveFrom(Window& other) noexcept;
};

#endif // WINDOW_HPP
