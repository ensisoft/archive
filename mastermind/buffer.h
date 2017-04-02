

#pragma once

#include <vector>
#include <cassert>
#include <cctype>
#include "bitset.h"

namespace mastermind
{
    enum class Color {
        None,

        Black,

        Red,

        Green,

        Yellow,

        Blue,

        Magenta,

        Cyan,

        White
    };

    enum class Hilites {
        Bold, 

        Dim,

        Underline,

        Blink,

        Standout,

        RevColor
    };

    using Hilite = Bitset<Hilites>;

    struct Cell 
    {
        // cell character value.
        wchar_t value;

        // cell text color
        Color  color;

        // cell text hightlite
        Hilite hilite;

        Cell() : value(0), color(Color::None)
        {}
    };

    // Buffer that holds our screen state in Cells.
    class Buffer
    {
    public:
        using Row = std::vector<Cell>;

        Buffer(std::size_t cols, std::size_t rows) : m_cols(cols), m_rows(rows)
        {
            resize(cols, rows);
        }
        
        void resize(std::size_t cols, std::size_t rows)
        {
            m_data.resize(rows);
            for (auto& row : m_data)
                row.resize(cols);
        }

        Row& operator[](std::size_t row)
        {
            assert(row < m_rows);
            return m_data[row];
        }

        const Row& operator[](std::size_t row) const 
        {
            assert(row < m_rows);
            return m_data[row];
        }

        void clear()
        {
            for (auto& row : m_data)
            {
                row.clear();
                row.resize(m_cols);
            }
        }

        std::size_t rows() const 
        { return m_rows; }

        std::size_t cols() const 
        { return m_cols; }

        void copy(const Buffer& other, std::size_t dst_x, std::size_t dst_y)
        {
            const auto cols = other.cols();
            const auto rows = other.rows();
            for (std::size_t y=0; y<rows; ++y)
            {
                if (y + dst_y >= m_rows)
                    break;
                for (std::size_t x=0; x<cols; ++x)
                {
                    if (x + dst_x >= m_cols)
                        break;
                    m_data[y + dst_y][x + dst_x] = other[y][x];
                }
            }
        }

    private:
        std::vector<Row> m_data;
        std::size_t m_cols;        
        std::size_t m_rows;
    };

} // mastermind
