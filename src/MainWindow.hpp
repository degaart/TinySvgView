#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "Window.hpp"
#include <plutosvg.h>

struct BitmapData
{
    int width;
    int height;
    int stride;
    void* data;
};

class MainWindow : public Window
{
public:
    explicit MainWindow(HINSTANCE hInstance);
    ~MainWindow();
    void registerWindowClass(HINSTANCE hInstance);
    void create();
    void openFile(const std::string& path);
private:
    static constexpr auto UM_OPEN_FILE = WM_USER + 100;
    static constexpr auto ID_FILE_OPEN = 101;
    static constexpr auto ID_FILE_EXIT = 102;

    std::string _filename;
    HBITMAP _backbuffer;
    plutosvg_document_t* _document;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void updateBackbuffer(HDC hdc);

    void onDestroy();
    void onPaint();
    void onOpenFile(const std::string& path);
    void onCommand(unsigned id);
    void onFileOpen();
    void onCreate();
    void onSize(int width, int height);
};

#endif // MAIN_WINDOW_HPP
