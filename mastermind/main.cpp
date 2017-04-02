
#include "view.h"
#include "game.h"
#include "welcome.h"
#include "terminal.h"
#include "buffer.h"
#include "cursor.h"
#include "gameview.h"
#include <memory>
#include <cstdlib>
#include <ctime>

namespace mastermind
{
    // global actions.
    enum class Action {
        None,

        Quit,

        NewGame,

    };

    // simple key map
    Action map_key(mastermind::Keysym sym)
    {
        switch (sym)
        {
            case Keysym::Key_Q:
                return Action::Quit;
            case Keysym::Key_N:
                return Action::NewGame;
            default:
                break;
        }
        return Action::None;
    }
}


int main(int, char* [])
{
    std::srand(std::time(nullptr));

    mastermind::Board board(12);
    mastermind::Game game(board);
    mastermind::Terminal term;
    mastermind::Cursor curs;

    mastermind::Buffer buff(80, 40);

    std::unique_ptr<mastermind::View> view(new mastermind::Welcome);

    bool play = true;
    while (play)
    {
        buff.clear();
        view->draw(buff, curs);
        term.display(buff);
        term.display(curs);

        const auto key = term.get_key();
        const auto act = mastermind::map_key(key);
        switch (act)
        {
            case mastermind::Action::Quit:
                play = false;
                break;

            case mastermind::Action::NewGame:
                view.reset(new mastermind::GameView(game));
                game.start([]{ return std::rand() >> 6; });
                break;

            case mastermind::Action::None:
                view->react(key);
                break;
        }
    }
    return 0;
}