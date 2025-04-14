#include "MainWindow.hpp"

#include "Debug.hpp"
#include "Menu.hpp"
#include "Util.hpp"
#include <commdlg.h>
#include <fcntl.h>
#include <io.h>

MainWindow::MainWindow(HINSTANCE hInstance)
    : Window(hInstance), _backbuffer(nullptr), _bitmapData{}
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
    case WM_COMMAND:
        onCommand(LOWORD(wParam));
        return 0;
    case UM_OPEN_FILE:
        onOpenFile(reinterpret_cast<const char*>(lParam));
        return 0;
    case WM_CREATE:
        onCreate();
        return 0;
    case WM_SIZE:
        onSize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    return DefWindowProc(getHandle(), uMsg, wParam, lParam);
}

void MainWindow::onDestroy()
{
    if (_backbuffer)
    {
        DeleteObject(_backbuffer);
        _backbuffer = nullptr;
    }
    PostQuitMessage(0);
}

void MainWindow::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(getHandle(), &ps);

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, _backbuffer);

    RECT clipRect;
    GetClipBox(hdc, &clipRect);

    // BitBlt only the visible portion
    BitBlt(hdc, 
           clipRect.left, clipRect.top,
           clipRect.right - clipRect.left, clipRect.bottom - clipRect.top,
           memDC,
           clipRect.left, clipRect.top,
           SRCCOPY);
    
    // Clean up
    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
    EndPaint(getHandle(), &ps);
}

void MainWindow::create()
{
    Menu mainMenu;

    Menu fileMenu;
    fileMenu.appendMenuItem("&Open", ID_FILE_OPEN);
    fileMenu.appendMenuItem("", 0, Menu::FLAG_SEPARATOR);
    fileMenu.appendMenuItem("E&xit", ID_FILE_EXIT);
    mainMenu.appendMenuItem("&File", fileMenu);

    Window::create(
        "MainWindow",                       // className
        "MainWindow",                       // title
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // style
        CW_USEDEFAULT,                      // x
        CW_USEDEFAULT,                      // y
        800,                                // width
        600,                                // height
        nullptr,                            // parent
        mainMenu.detach()                   // menu
    );
}

void MainWindow::registerWindowClass(HINSTANCE hInstance)
{
    Window::registerWindowClass(
        hInstance,                          // hInstance
        CS_HREDRAW | CS_VREDRAW,            // style
        nullptr,                            // icon
        LoadCursor(nullptr, IDC_ARROW),     // cursor
        (HBRUSH)(COLOR_BTNFACE + 1),         // background
        nullptr,                            // menuName
        "MainWindow",                       // className
        nullptr                             // iconSm
    );
}

void MainWindow::openFile(const std::string& path)
{
    SendMessage(getHandle(), UM_OPEN_FILE, 0, reinterpret_cast<LPARAM>(path.c_str()));
}

void MainWindow::onCommand(unsigned id)
{
    switch (id)
    {
    case ID_FILE_OPEN:
        onFileOpen();
        break;
    case ID_FILE_EXIT:
        SendMessage(getHandle(), WM_CLOSE, 0, 0);
        break;
    }
}

void MainWindow::onFileOpen()
{
    trace("onFileOpen");

    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = _hwnd;
    ofn.lpstrFilter = "SVG files (*.svg)\0*.svg\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = (LPSTR) malloc(MAX_PATH);
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    ofn.lpstrFile[0] = '\0';
    if (!GetOpenFileName(&ofn))
    {
        auto msg = getErrorMessage(GetLastError());
        trace("GetOpenFileName failed: {}", msg);

        free(ofn.lpstrFile);
        return;
    }

    trace(fmt::format("Open file: {}", ofn.lpstrFile));
    openFile(ofn.lpstrFile);
    free(ofn.lpstrFile);
}

void MainWindow::onOpenFile(const std::string& path)
{
    _filename = path;
    trace("onOpenFile: {}", _filename);
}

void MainWindow::onCreate()
{
    static constexpr auto WIDTH = 353;
    static constexpr auto HEIGHT = 480;
    static constexpr auto STRIDE = 353 * 3;

    int fd = _open("testdata.raw", _O_RDONLY|_O_BINARY);
    if (fd == -1)
    {
        trace("Failed to open testdata.raw");
        return;
    }

    auto bufferSize = STRIDE * HEIGHT;
    unsigned char* buffer = (unsigned char*) malloc(bufferSize);
    auto ret = _read(fd, buffer, bufferSize);
    if (ret != bufferSize)
    {
        trace("Failed to read testdata.raw");
        _close(fd);
        return;
    }
    _close(fd);
    trace("File loaded successfully");

    _bitmapData.width = WIDTH;
    _bitmapData.height = HEIGHT;
    _bitmapData.stride = STRIDE;
    _bitmapData.data = buffer;

    auto hdc = GetDC(_hwnd);
    updateBackbuffer(hdc);
    ReleaseDC(_hwnd, hdc);
}

void MainWindow::updateBackbuffer(HDC hdc)
{
    auto memDC = CreateCompatibleDC(hdc);
    RECT rcClient;
    GetClientRect(_hwnd, &rcClient);
    _backbuffer = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    if (!_backbuffer)
    {
        trace("Failed to create backbuffer");
        DeleteDC(memDC);
        ReleaseDC(_hwnd, hdc);
    }

    auto oldBitmap = (HBITMAP)SelectObject(memDC, _backbuffer);
    FillRect(memDC, &rcClient, (HBRUSH)GetStockObject(DKGRAY_BRUSH));

    const unsigned char* buffer = static_cast<const unsigned char*>(_bitmapData.data);
    for (int y = 0; y < _bitmapData.height && y < rcClient.bottom - rcClient.top; y++)
    {
        for (int x = 0; x < _bitmapData.width && x < rcClient.right - rcClient.left; x++)
        {
            auto index = y * _bitmapData.stride + x * 3;
            auto r = buffer[index];
            auto g = buffer[index + 1];
            auto b = buffer[index + 2];
            auto color = RGB(r, g, b);
            SetPixel(memDC, x, y, color);
        }
    }

    SelectObject(memDC, oldBitmap);
    DeleteDC(memDC);
}

void MainWindow::onSize(int width, int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    auto hdc = GetDC(_hwnd);
    updateBackbuffer(hdc);
    ReleaseDC(_hwnd, hdc);
}
