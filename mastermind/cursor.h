

#pragma once

#include <cstddef>

namespace mastermind
{
    class Cursor 
    {
    public:
        Cursor() : m_xpos(0), m_ypos(0), m_visible(false)
        {}

        std::size_t x() const 
        { return m_xpos; }

        std::size_t y() const 
        { return m_ypos; }

        void move(std::size_t x, std::size_t y)
        {
            m_xpos = x;
            m_ypos = y;
        }

        bool visible() const 
        { return m_visible; }

        void show() 
        {
            m_visible = true;
        }

        void hide()
        {
            m_visible = false;
        }
    private:
        std::size_t m_xpos;
        std::size_t m_ypos;
    private:
        bool m_visible;
    };
} // mastermind