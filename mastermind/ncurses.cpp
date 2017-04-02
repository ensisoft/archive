
#include "terminal.h"
#include "buffer.h"
#include "cursor.h"
#include <curses.h>
#include <map>
#include <vector>
#include <algorithm>

namespace mastermind
{

struct Terminal::Impl {
    // nothing here yet.
};

Terminal::Terminal() : m_impl(new Impl)
{
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    raw();
    curs_set(0);

    init_pair((short)Color::Black,   COLOR_BLACK,   COLOR_BLACK);
    init_pair((short)Color::Red,     COLOR_RED,     COLOR_BLACK);
    init_pair((short)Color::Green,   COLOR_GREEN,   COLOR_BLACK);
    init_pair((short)Color::Yellow,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair((short)Color::Blue,    COLOR_BLUE,    COLOR_BLACK);
    init_pair((short)Color::Magenta, COLOR_MAGENTA, COLOR_BLACK);
    init_pair((short)Color::Cyan,    COLOR_CYAN,    COLOR_BLACK);
    init_pair((short)Color::White,   COLOR_WHITE,   COLOR_BLACK);
}

Terminal::~Terminal()
{
    endwin();
}

void Terminal::display(const Cursor& cursor)
{
    if (!cursor.visible())
    {
        curs_set(0);
    }
    else
    {
        move(cursor.y(), cursor.x());        
        curs_set(1);
        refresh();
    }
}

void Terminal::display(const Buffer& buff)
{
    const auto rows = buff.rows();
    const auto cols = buff.cols();

    clear();

    for (std::size_t y=0; y<rows; ++y)
    {
        for (std::size_t x=0; x<cols; ++x)
        {
            const auto& cell = buff[y][x];
            if (cell.value == 0)
                continue;

            if (cell.hilite.test(Hilites::Bold))      attron(A_BOLD);
            if (cell.hilite.test(Hilites::Dim))       attron(A_DIM);
            if (cell.hilite.test(Hilites::Underline)) attron(A_UNDERLINE);
            if (cell.hilite.test(Hilites::Blink))     attron(A_BLINK);
            if (cell.hilite.test(Hilites::Standout))  attron(A_STANDOUT);
            if (cell.hilite.test(Hilites::RevColor))  attron(A_REVERSE);
            if (cell.color != Color::None) attron(COLOR_PAIR((short)cell.color));

            move(y, x);
            printw("%c", cell.value);

            if (cell.hilite.test(Hilites::Bold))      attroff(A_BOLD);
            if (cell.hilite.test(Hilites::Dim))       attroff(A_DIM);
            if (cell.hilite.test(Hilites::Underline)) attroff(A_UNDERLINE);
            if (cell.hilite.test(Hilites::Blink))     attroff(A_BLINK);
            if (cell.hilite.test(Hilites::Standout))  attroff(A_STANDOUT);            
            if (cell.hilite.test(Hilites::RevColor))  attroff(A_REVERSE);
            if (cell.color != Color::None) attroff(COLOR_PAIR((short)cell.color));
        }
    }
    refresh();
}

Keysym Terminal::get_key() const 
{
    const static std::map<int, Keysym> map {
        {KEY_BACKSPACE, Keysym::Backspace},
        {'\t',          Keysym::Tab},
        {'\n',          Keysym::Enter},
        {' ',           Keysym::Space},
        {'0',           Keysym::Key_0},
        {'1',           Keysym::Key_1},
        {'2',           Keysym::Key_2},
        {'3',           Keysym::Key_3},
        {'4',           Keysym::Key_4},
        {'5',           Keysym::Key_5},
        {'6',           Keysym::Key_6},
        {'7',           Keysym::Key_7},
        {'8',           Keysym::Key_8},
        {'9',           Keysym::Key_9},
        {'a',           Keysym::Key_A},
        {'b',           Keysym::Key_B},
        {'c',           Keysym::Key_C},
        {'d',           Keysym::Key_D},
        {'e',           Keysym::Key_E},
        {'f',           Keysym::Key_F},
        {'g',           Keysym::Key_G},
        {'h',           Keysym::Key_H},
        {'i',           Keysym::Key_I},
        {'j',           Keysym::Key_J},
        {'k',           Keysym::Key_K},
        {'l',           Keysym::Key_L},
        {'m',           Keysym::Key_M},
        {'n',           Keysym::Key_N},
        {'o',           Keysym::Key_O},
        {'p',           Keysym::Key_P},
        {'q',           Keysym::Key_Q},                        
        {'r',           Keysym::Key_R},        
        {'s',           Keysym::Key_S},        
        {'t',           Keysym::Key_T},                
        {'u',           Keysym::Key_U},                        
        {'v',           Keysym::Key_V},                                
        {'w',           Keysym::Key_W},                                        
        {'x',           Keysym::Key_X},
        {'y',           Keysym::Key_Y},               
        {'z',           Keysym::Key_Z},               
        {KEY_F(1),      Keysym::F1},
        {KEY_F(2),      Keysym::F2},
        {KEY_F(3),      Keysym::F3},
        {KEY_F(4),      Keysym::F4},
        {KEY_F(5),      Keysym::F5},
        {KEY_F(6),      Keysym::F6},
        {KEY_F(7),      Keysym::F7},
        {KEY_F(8),      Keysym::F8},
        {KEY_F(9),      Keysym::F9},
        {KEY_F(10),     Keysym::F10},
        {KEY_F(11),     Keysym::F11},                                                                               
        {KEY_F(12),     Keysym::F12},       
        {KEY_IC,        Keysym::Insert},
        {KEY_DC,        Keysym::Delete},
        {KEY_HOME,      Keysym::Home},
        {KEY_END,       Keysym::End},
        {KEY_NPAGE,     Keysym::PageDown},
        {KEY_PPAGE,     Keysym::PageUp},
        {KEY_LEFT,      Keysym::Left},
        {KEY_UP,        Keysym::Up},
        {KEY_DOWN,      Keysym::Down},
        {KEY_RIGHT,     Keysym::Right},
        {0x1b,          Keysym::Escape}
    };

    int ch = ::getch();
    if (std::isalpha(ch))
        ch = std::tolower(ch);

    const auto it = map.find(ch);
    if (it == std::end(map))
        return Keysym::None;

    return it->second;
}

} // mastermind