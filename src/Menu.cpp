#include "Menu.hpp"

#include "Debug.hpp"
#include "Util.hpp"
#include <fmt/format.h>

Menu::Menu()
    : _menu(CreateMenu()), _detached(false)
{
}

Menu::~Menu()
{
    if (_menu && !_detached)
    {
        DestroyMenu(_menu);
    }
}

Menu::Menu(Menu&& other) noexcept
    : _menu(nullptr), _detached(false)
{
    moveFrom(other);
}

Menu& Menu::operator=(Menu&& other) noexcept
{
    if (this != &other)
    {
        if (!_menu && !_detached)
        {
            DestroyMenu(_menu);
        }
        _menu = nullptr;
        _detached = false;

        moveFrom(other);
    }
    return *this;
}

void Menu::moveFrom(Menu& other) noexcept
{
    _menu = other._menu;
    _detached = other._detached;
    other._menu = nullptr;
    other._detached = false;
}

HMENU Menu::getHandle() const
{
    return _menu;
}

HMENU Menu::detach()
{
    _detached = true;
    return _menu;
}

void Menu::insertMenuItem(
    unsigned item,
    bool byPosition,
    const std::string& text,
    Menu* submenu,
    unsigned id,
    unsigned flags
)
{
    MENUITEMINFO ii = {};
    ii.cbSize = sizeof(MENUITEMINFO);
    ii.fMask = MIIM_STRING | MIIM_ID | MIIM_FTYPE;
    ii.fState = 0;
    ii.wID = id;
    ii.hbmpChecked = nullptr;
    ii.hbmpUnchecked = nullptr;
    ii.dwItemData = 0;

    if (submenu)
    {
        ii.fMask |= MIIM_SUBMENU;
        ii.hSubMenu = submenu->detach();
    }

    if (flags & FLAG_SEPARATOR)
    {
        ii.fType = MFT_SEPARATOR;
    }
    else
    {
        ii.dwTypeData = const_cast<LPSTR>(text.c_str());
        ii.cch = text.length();
        ii.fType = MFT_STRING;
    }

    auto ret = InsertMenuItem(_menu, item, byPosition, &ii);
    if (!ret)
    {
        throw std::runtime_error(fmt::format("InsertMenuItem failed: {}", GetLastError()));
    }
}

void Menu::insertMenuItem(MenuPosition beforePosition, const std::string& text, unsigned id, unsigned flags)
{
    insertMenuItem(beforePosition.get(), true, text, nullptr, id, flags);
}

void Menu::insertMenuItem(MenuID beforeID, const std::string& text, unsigned id, unsigned flags)
{
    insertMenuItem(beforeID.get(), false, text, nullptr, id, flags);
}

void Menu::appendMenuItem(const std::string& text, unsigned id, unsigned flags)
{
    insertMenuItem(UINT_MAX, true, text, nullptr, id, flags);
}

void Menu::appendMenuItem(const std::string& text, Menu& menu)
{
    insertMenuItem(UINT_MAX, true, text, &menu, 0, 0);
}

