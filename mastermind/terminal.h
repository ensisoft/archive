

#pragma once

#include <memory>
#include "keys.h"

namespace mastermind
{
    class Buffer;
    class Cursor;

    // Access the terminal.
    class Terminal
    {
    public:
        Terminal();
       ~Terminal();

        // Display the contents of the buffer on the terminal screen.
        void display(const Buffer& buff);

        void display(const Cursor& cursor);
        
        // Read an input key. Will block untill a new key is available.
        Keysym get_key() const;

    private:
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    };

} // mastermind
