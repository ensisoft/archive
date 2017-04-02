
#pragma once

#include "buffer.h"
#include <string>

namespace mastermind
{
    // Format text into a Buffer
    class Formatter
    {
    public:
        enum class Align {
            Left, Center, Right
        };

        Formatter(Buffer& buff) : m_buff(buff), m_xpos(0), m_ypos(0), m_align(Align::Left)
        {}

        void print(std::size_t row, const std::string& text)
        {
            print(row, text, m_align);
        }

        void print(std::size_t row, const std::string& text, Align align)
        {
            const auto cols = m_buff.cols();

            if (align == Align::Left)
            {
                for (std::size_t x=0; x<cols; ++x)
                {
                    if (x == text.size())
                        break;
                    auto& cell = m_buff[row][x];
                    cell.hilite = m_hilites;
                    cell.value  = text[x];
                }
            }
            else if (align == Align::Right)
            {
                std::size_t i=text.size() - 1;
                for (std::size_t x=cols; x > 0; --x, --i)
                {
                    auto& cell = m_buff[row][x-1];
                    cell.hilite = m_hilites;
                    cell.value  = text[i];
                    if (i == 0) break;
                }
            }
            else if (align == Align::Center)
            {
                const auto chars  = std::min(text.size(), cols);
                const auto offset = (cols - chars) / 2;
                for (std::size_t x=0; x<chars; ++x)
                {
                    auto& cell = m_buff[row][x + offset];
                    cell.hilite = m_hilites;
                    cell.value  = text[x];
                }
            }
        }

        void move(std::size_t x, std::size_t y)
        {
            m_xpos = x;
            m_ypos = y;
        }

        void set(Hilites hilite, bool on_off)
        {
            m_hilites.set(hilite, on_off);
        }

        void enable(Hilites hilite)
        {
            set(hilite, true);
        }
        void disable(Hilites hilite)
        {
            set(hilite, false);
        }

        void align(Align a)
        {
            m_align = a;
        }
        void clear()
        {
            m_hilites.clear();
            m_align = Align::Left;
        }

    private:
        Buffer& m_buff;
    private:
        std::size_t m_xpos;
        std::size_t m_ypos;
        Align m_align;
    private:
        Hilite m_hilites;

    };

} // mastermind
