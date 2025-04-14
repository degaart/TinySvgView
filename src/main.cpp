#include "Debug.hpp"
#include "Util.hpp"
#include "MainWindow.hpp"
#include <fmt/format.h>
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    trace("Started");

    auto args = splitCommandLine(lpCmdLine);
    for (const auto& arg : args)
    {
        trace(fmt::format("Arg: {}", arg));
    }

    MainWindow mainWindow(hInstance);
    mainWindow.registerWindowClass(hInstance);
    mainWindow.create();
    mainWindow.show(nCmdShow);
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    trace("Exited");
    return 0;
}
