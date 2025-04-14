#ifndef TRACE_WINDOW_HPP
#define TRACE_WINDOW_HPP

#include "Window.hpp"
#include "Font.hpp"

class TraceWindow : public Window
{
public:
    static constexpr auto UM_TRACE = WM_USER + 100;

    explicit TraceWindow(HINSTANCE hInstance);
    ~TraceWindow();

    void registerWindowClass(HINSTANCE hInstance);
    void create();
private:
    HWND _editCtl;
    Font _font;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void onCreate();
    void onSize();
    void onTrace(const std::string& message);
    void onDestroy();
};

#endif // TRACE_WINDOW_HPP
