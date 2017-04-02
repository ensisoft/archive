

#pragma once

#include "keys.h"

namespace mastermind
{

    class Buffer;
    class Cursor;

    class View 
    {
    public:
        virtual ~View() = default;

        // Draw the contents of the view into the buffer.
        virtual void draw(Buffer& buff, Cursor& curs) = 0;

        virtual void react(Keysym key) = 0;

    private:

    };

} // mastermind