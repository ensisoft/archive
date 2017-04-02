

#pragma once

#include "view.h"

namespace mastermind
{
    class Welcome : public View
    {
    public:
        virtual void draw(Buffer& buff, Cursor& cursor) override;

        virtual void react(Keysym sym) override;

    private:
    };


} // mastermind