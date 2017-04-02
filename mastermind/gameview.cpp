

#include "gameview.h"
#include "game.h"
#include "buffer.h"
#include "cursor.h"
#include "formatter.h"
#include <map>

namespace mastermind
{

Cell format(Peg p)
{
    Cell cell;
    cell.value = 'O';
    //cell.hilite.set(Hilites::RevColor, true);
    switch (p)
    {
        case Peg::None: 
            cell.value = '_';
            break;

        case Peg::Red:
            cell.value = 'R';
            cell.color = Color::Red;
            break;        

        case Peg::Green:
            cell.value = 'G';
            cell.color = Color::Green;
            break;

        case Peg::Orange:
            cell.value = 'O';
            cell.color = Color::Yellow;
            break;

        case Peg::Blue:
            cell.value = 'B';
            cell.color = Color::Blue;
            break;

        case Peg::Cyan: 
            cell.value = 'C';
            cell.color = Color::Cyan;
            break;

        case Peg::White:
            cell.value = 'W';
            cell.color = Color::White;
            break;
    }
    return cell;
}

Cell format(Pin p)
{
    Cell cell;
    cell.value = 'X';
    switch (p)
    {
        case Pin::None: 
            cell.value = '_';
            break;

        case Pin::Black:
            cell.value = 'b';
            break;

        case Pin::White:
            cell.value = 'w';
            break;
    }
    return cell;
}

void format(Buffer& buff, const Game::Guess& guess, std::size_t y)
{
    auto& row = buff[y];
    row[0] = format(guess.pegs[0]);
    row[2] = format(guess.pegs[1]);
    row[4] = format(guess.pegs[2]);
    row[6] = format(guess.pegs[3]);
}

void format(Buffer& buff, const Board::Slot& slot, std::size_t y)
{
    auto& row = buff[y];
    row[0]  = format(slot.pegs[0]);
    row[2]  = format(slot.pegs[1]);
    row[4]  = format(slot.pegs[2]);
    row[6]  = format(slot.pegs[3]);

    row[9]  = format(slot.pins[0]);
    row[10] = format(slot.pins[1]);    
    row[11] = format(slot.pins[2]);    
    row[12] = format(slot.pins[3]);    
}

GameView::GameView(Game& game) : m_game(game)
{
    m_message = "Let's play!";
    m_secret  = true;
}

void GameView::draw(Buffer& buff, Cursor& cursor)
{

    Formatter fmt(buff);
    fmt.align(Formatter::Align::Center);
    fmt.set(Hilites::Bold, true);

    if (m_secret)
    {
        fmt.print(3, "S E C R E T");
    }
    else
    {
        const auto code = m_game.get_code();
        const auto xpos = (buff.cols() - 7) / 2;
        buff[3][xpos + 0] = format(code[0]);
        buff[3][xpos + 2] = format(code[1]);
        buff[3][xpos + 4] = format(code[2]);
        buff[3][xpos + 6] = format(code[3]);
    }
    fmt.print(4, "-----------");
    fmt.print(6, "Pegs:    Pins:");

    const auto& board    = m_game.get_board();
    const auto max_guess = m_game.max_guesses();
    const auto cur_guess = m_game.cur_guess();

    // 4 pegs + 3 spaces + 2 spaces + 4 pins
    Buffer slots(14, max_guess);

    for (std::size_t i=0; i<max_guess; ++i)
    {
        const auto& slot = board[i];
        format(slots, slot, i);
    }

    if (cur_guess < max_guess)
        format(slots, m_guess, cur_guess);

    const auto x = (buff.cols() - slots.cols()) / 2;
    const auto y = 8;

    buff.copy(slots, x, y);

    if (m_game.is_running())
    {
        cursor.show();
        if (m_guess.peg < 4)
            cursor.move(x + m_guess.peg * 2, y + cur_guess);        
    }
    else
    {
        cursor.hide();
    }

    fmt.clear();
    fmt.align(Formatter::Align::Center);
    fmt.print(8 + max_guess + 1, m_message);

    fmt.align(Formatter::Align::Left);
    fmt.print(8,  "  Press ...");
    fmt.print(10, "  'R' for Red peg.");
    fmt.print(11, "  'G' for Green peg.");
    fmt.print(12, "  'O' for Orange peg.");
    fmt.print(13, "  'B' for Blue peg.");    
    fmt.print(14, "  'C' for Cyan peg.");        
    fmt.print(15, "  'W' for White peg.");
    fmt.print(17, "  'Q' to Quit.");
    fmt.print(18, "  'N' for New Game");

}

void GameView::react(Keysym sym)
{
    if (!m_game.is_running())
        return;

    static const std::map<Keysym, std::pair<Peg, std::string>> map = {
        {Keysym::Key_R, {Peg::Red,    "Red"}},
        {Keysym::Key_G, {Peg::Green,  "Green"}},
        {Keysym::Key_O, {Peg::Orange, "Orange"}},
        {Keysym::Key_B, {Peg::Blue,   "Blue"}},
        {Keysym::Key_C, {Peg::Cyan,   "Cyan"}},
        {Keysym::Key_W, {Peg::White,  "White"}}
    };

    if (sym == Keysym::Backspace)
    {
        m_guess.pop_peg();
    }
    else if (sym == Keysym::Enter)
    {
        if (m_guess.is_ready())
        {
            const auto feedback = m_game.play(m_guess);
            m_guess.clear();
            m_message.clear();

            const auto whites = feedback.num_pins(Pin::White);
            const auto blacks = feedback.num_pins(Pin::Black);
            if (blacks)
            {
                const char* msgs[] = {
                    "Great job!",
                    "Fantastic!",
                    "Hell yeah!",
                    "Well done!",
                    "Awsum bruh!",
                    "You rock!"
                };

                m_message = msgs[std::rand() % 6]; 
            }
            else if (whites)
            {
                const char* msgs[] = {
                    "Something ...",
                    "Not bad.",
                    "Okay ...",
                    "So what's next?",
                    "Hmhh ..."
                };

                m_message = msgs[std::rand() % 5];
            }
            else
            {
                const char* msgs[] = {
                    "Bad luck dude.",
                    "Oh, thats a dud!",
                    "Nothing ...",
                    "Ah, man!",
                    "C'mon!",
                    "Well that sucks."
                };

                m_message = msgs[std::rand() % 6];
            }
        }
        const auto state = m_game.get_state();
        if (state == Game::State::Win)
        {
            m_message = "You Win!!";
            m_secret  = false;
        }
        else if (state == Game::State::Loose)
        {
            m_message = "Sorry, you lost!";
            m_secret  = false;
        }
    }
    else if (sym == Keysym::F12)
    {
        m_secret = false;
    }
    else if (sym == Keysym::Left)
    {
        m_guess.go_left();
    }
    else if (sym == Keysym::Right)
    {
        m_guess.go_right();
    }
    else
    {
        const auto it = map.find(sym);
        if (it == std::end(map))
        {
            m_message = "What?";
            return;
        }
        const auto& pair = it->second;
        m_guess.set_peg(pair.first);
        m_guess.go_right();
        m_message = "You selected " + pair.second + " peg.";
        if (m_guess.is_ready())
        {
            m_message += " Press Enter to Guess";
        }

    }
}

} //  mastermind
