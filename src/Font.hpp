#ifndef FONT_HPP
#define FONT_HPP

#include <Windows.h>
#include <string>

class Font
{
public:
    static constexpr auto BOLD = (1 << 0);
    static constexpr auto ITALIC = (1 << 1);
    static constexpr auto UNDERLINE = (1 << 2);
    static constexpr auto STRIKEOUT = (1 << 3);

    Font(HWND hWnd, int size, const std::string& name, unsigned flags = 0);
    Font(Font&&) noexcept;
    Font(const Font&) = delete;
    Font& operator=(Font&&) noexcept;
    Font& operator=(const Font&) = delete;
    ~Font();
    
    HFONT getHandle() const noexcept { return _hFont; }
private:
    HFONT _hFont;

    void moveFrom(Font& other) noexcept;
};

#endif // FONT_HPP
