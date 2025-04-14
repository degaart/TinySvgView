#include "Font.hpp"

#include "Util.hpp"
#include <fmt/format.h>
#include <stdexcept>


Font::Font(HWND hWnd, int size, const std::string& name, unsigned flags)
    : _hFont(nullptr)
{
    auto dc = GetDC(hWnd);
    if (!dc)
    {
        throw std::runtime_error(fmt::format("GetDC failed: {}", getErrorMessage(GetLastError())));
    }

    LOGFONT lf = {};
    lf.lfHeight = -MulDiv(size, GetDeviceCaps(dc, LOGPIXELSY), 72);
    lf.lfWeight = (flags & BOLD) ? FW_BOLD : FW_DONTCARE;
    lf.lfItalic = (flags & ITALIC) ? TRUE : FALSE;
    lf.lfUnderline = (flags & UNDERLINE) ? TRUE : FALSE;
    lf.lfStrikeOut = (flags & STRIKEOUT) ? TRUE : FALSE;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH;
    strcpy_s(lf.lfFaceName, sizeof(lf.lfFaceName), name.c_str());

    _hFont = CreateFontIndirect(&lf);
    if (!_hFont)
    {
        auto msg = getErrorMessage(GetLastError());
        ReleaseDC(hWnd, dc);
        throw std::runtime_error(fmt::format("CreateFontIndirect failed: {}", msg));
    }

    ReleaseDC(hWnd, dc);   
}

Font::Font(Font&& other) noexcept
    : _hFont(nullptr)
{
    moveFrom(other);
}

Font& Font::operator=(Font&& other) noexcept
{
    if (this != &other)
    {
        _hFont = nullptr;
        moveFrom(other);
    }
    return *this;
}

Font::~Font()
{
    if (_hFont)
    {
        DeleteObject(_hFont);
    }
}

void Font::moveFrom(Font& other) noexcept
{
    _hFont = other._hFont;
    other._hFont = nullptr;
}
