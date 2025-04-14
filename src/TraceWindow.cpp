#include "TraceWindow.hpp"

#include "Debug.hpp"
#include "Util.hpp"

TraceWindow::TraceWindow(HINSTANCE hInstance)
    : Window(hInstance),
      _editCtl(nullptr),
      _font(_hwnd, 12, "Consolas")
{
}

TraceWindow::~TraceWindow()
{
}

void TraceWindow::registerWindowClass(HINSTANCE hInstance)
{
    Window::registerWindowClass(
        hInstance,
        CS_HREDRAW | CS_VREDRAW,
        nullptr,
        LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        nullptr,
        "TraceWindow",
        nullptr
    );
}

LRESULT TraceWindow::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        onCreate();
        return 0;
    case WM_SIZE:
        onSize();
        return 0;
    case WM_DESTROY:
        onDestroy();
        return 0;
    case UM_TRACE:
        onTrace(reinterpret_cast<const char*>(lParam));
        return 0;
    }
    return DefWindowProc(getHandle(), msg, wParam, lParam);
}

void TraceWindow::create()
{
    Window::create(
        "TraceWindow",
        "Trace Window",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 240,
        nullptr,
        nullptr
    );
}

void TraceWindow::onCreate()
{
    RECT rcParent;
    GetClientRect(_hwnd, &rcParent);

    int width = rcParent.right - rcParent.left - 20;
    int height = rcParent.bottom - rcParent.top - 20;

    unsigned style =
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER |
        ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | WS_HSCROLL | ES_NOHIDESEL;
    
    _editCtl = CreateWindowEx(
        0,
        "EDIT",
        "",
        style,
        10, 10,
        width, height,
        _hwnd,
        nullptr,
        _hInstance,
        nullptr
    );

    if (!_editCtl)
    {
        throw std::runtime_error(fmt::format("Failed to create edit control: {}", getErrorMessage(GetLastError())));
    }

    //SendMessage(_editCtl, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 10);
    SendMessage(_editCtl, WM_SETFONT, reinterpret_cast<WPARAM>(_font.getHandle()), TRUE);
}

void TraceWindow::onSize()
{
    RECT rcParent;
    GetClientRect(_hwnd, &rcParent);

    int width = rcParent.right - rcParent.left - 20;
    int height = rcParent.bottom - rcParent.top - 20;

    MoveWindow(_editCtl, 10, 10, width, height, TRUE);
}

void TraceWindow::onTrace(const std::string& message)
{
    size_t size = SendMessage(_editCtl, WM_GETTEXTLENGTH, 0 , 0);
    size += message.size() + 16;

    auto buffer = new char[size];
    SendMessage(_editCtl, WM_GETTEXT, size, (LPARAM)buffer);

    strcat(buffer, message.c_str());
    strcat(buffer, "\r\n");
    SendMessage(_editCtl, WM_SETTEXT, 0, (LPARAM)buffer);

    SendMessage(_editCtl, EM_SETSEL, strlen(buffer), strlen(buffer));
    SendMessage(_editCtl, EM_SCROLLCARET, 0, 0);

    delete[] buffer;
}

void TraceWindow::onDestroy()
{
    setTraceWindow(nullptr);
}

