#ifndef MENU_HPP
#define MENU_HPP

#include <Windows.h>
#include <string>

class MenuID
{
public:
    explicit MenuID(unsigned id) : _id(id) {}
    operator unsigned() const { return _id; }
    unsigned get() const { return _id; }
private:
    unsigned _id;
};

class MenuPosition
{
public:
    MenuPosition(unsigned position) : _position(position) {}
    operator unsigned() const { return _position; }
    unsigned get() const { return _position; }
private:
    unsigned _position;
};

class Menu
{
public:
    static constexpr unsigned FLAG_SEPARATOR = (1 << 0);

    Menu();
    Menu(Menu&& other) noexcept;
    Menu& operator=(Menu&& other) noexcept;
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    ~Menu();

    HMENU getHandle() const;

    /*
        Resources associated with a menu that is assigned to a window are freed automatically.
        This function detaches the menu from the window and allows it to be used independently.
    */
    HMENU detach();
    void appendMenuItem(const std::string& text, unsigned id = 0, unsigned flags = 0);
    void appendMenuItem(const std::string& text, Menu& menu);
    void insertMenuItem(MenuPosition beforePosition, const std::string& text, unsigned id = 0, unsigned flags = 0);
    void insertMenuItem(MenuID beforeID, const std::string& text, unsigned id = 0, unsigned flags = 0);
private:
    HMENU _menu;
    bool _detached;

    void insertMenuItem(
        unsigned item,
        bool byPosition,
        const std::string& text,
        Menu* subMenu,
        unsigned id,
        unsigned flags
    );
    void moveFrom(Menu& other) noexcept;
};

#endif // MENU_HPP
