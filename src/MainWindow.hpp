#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "Window.hpp"

class MainWindow : public Window
{
public:
    MainWindow(HINSTANCE hInstance);
    ~MainWindow();
    void registerWindowClass(HINSTANCE hInstance);
    void create();
private:
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void onDestroy();
    void onPaint();
};

#endif // MAIN_WINDOW_HPP
