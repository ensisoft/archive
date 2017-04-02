
#pragma once

#include "view.h"
#include "game.h"
#include <cstddef>
#include <string>

namespace mastermind
{
    class GameView : public View
    {
    public:
        GameView(Game& game);

        virtual void draw(Buffer& buff, Cursor& cursor) override;

        virtual void react(Keysym sym) override;

    private:
        Game& m_game;
        Game::Guess m_guess;
    private:
        std::string m_message;
    private:
        bool m_secret;
    };

} // mastermind