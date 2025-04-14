#include "MainWindow.hpp"

#include "Debug.hpp"

MainWindow::MainWindow(HINSTANCE hInstance)
    : Window(hInstance)
{
}

MainWindow::~MainWindow()
{
}

LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        onDestroy();
        return 0;
    case WM_PAINT:
        onPaint();
        return 0;
    case WM_LBUTTONUP:
        trace("Click!");
        return 0;
    }
    return DefWindowProc(getHandle(), uMsg, wParam, lParam);
}

void MainWindow::onDestroy()
{
    PostQuitMessage(0);
}

void MainWindow::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(getHandle(), &ps);
    EndPaint(getHandle(), &ps);
}

void MainWindow::create()
{
    Window::create(
        "MainWindow",                       // className
        "MainWindow",                      // title
        WS_OVERLAPPEDWINDOW,                // style
        CW_USEDEFAULT,                      // x
        CW_USEDEFAULT,                      // y
        800,                                // width
        600,                                // height
        nullptr,                            // parent
        nullptr                             // menu
    );
}

void MainWindow::registerWindowClass(HINSTANCE hInstance)
{
    Window::registerWindowClass(
        hInstance,                          // hInstance
        CS_HREDRAW | CS_VREDRAW,            // style
        nullptr,                            // icon
        LoadCursor(nullptr, IDC_ARROW),     // cursor
        (HBRUSH)(COLOR_WINDOW + 1),         // background
        nullptr,                            // menuName
        "MainWindow",                       // className
        nullptr                             // iconSm
    );
}
