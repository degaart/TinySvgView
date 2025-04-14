#include "Debug.hpp"
#include "Util.hpp"
#include "MainWindow.hpp"
#include "TraceWindow.hpp"
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

    TraceWindow traceWindow(hInstance);
    traceWindow.registerWindowClass(hInstance);
    traceWindow.create();
    traceWindow.show(nCmdShow);
    setTraceWindow(traceWindow.getHandle());

    trace("Trace window created");

    MainWindow mainWindow(hInstance);
    mainWindow.registerWindowClass(hInstance);
    mainWindow.create();
    mainWindow.show(nCmdShow);

    if (args.size() > 1)
    {
        for (const auto& arg : args)
        {
            mainWindow.openFile(arg);
        }
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    trace("Exited");
    return 0;
}
