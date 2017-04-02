

#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <array>
#include <cassert>

// note a gcc quirk here that 5.2.0 gcc will not compile a brace-initializer list
// in a struct without duplicated braces.

namespace mastermind
{
    // Gaming pegs for forming the game code.
    enum class Peg {
        None, Red, Green, Orange, Blue, Cyan, White
    };

    // Feedback pins
    enum class Pin {
        None,
        // Indicates that a correct peg was at a correct place.
        Black,
        // Indicates that a correct peg was at incorrect place.
        White
    };


    // The decoding board, keeps track of the attempts to solve the game.
    class Board
    {
    public:
        struct Slot {
            std::array<Peg, 4> pegs {{Peg::None}};
            std::array<Pin, 4> pins {{Pin::None}};
        };

        Board(std::size_t rows) : m_size(rows)
        {
            m_slots.resize(rows);
        }
        std::size_t size() const 
        {
            return m_size;
        }

        const Slot& operator[](std::size_t i) const 
        {
            assert(i < m_slots.size());
            return m_slots[i];
        }

        Slot& operator[](std::size_t i) 
        {
            assert(i < m_slots.size());
            return m_slots[i];
        }

        void clear()
        {
            m_slots.clear();
            m_slots.resize(m_size);
        }

    private:
        std::vector<Slot> m_slots;
        std::size_t m_size;
    };


    // Implements MasterMind game logic.
    class Game
    {
    public:
        enum class State {
            None, Play, Win, Loose
        };

        struct Guess {
            std::size_t peg {0};
            std::array<Peg, 4> pegs {{Peg::None}};

            void clear()
            {
                pegs = {{Peg::None}};
                peg  = 0;
            }
            void set_peg(Peg p)
            {
                pegs[peg] = p;
            }

            void add_peg(Peg p)
            {
                if (peg < pegs.size())
                    pegs[peg++] = p;
            }
            void pop_peg()
            {
                if (peg > 0)
                    pegs[--peg] = Peg::None;
            }
            bool is_ready() const 
            {
                return pegs[0] != Peg::None &&
                       pegs[1] != Peg::None &&
                       pegs[2] != Peg::None &&
                       pegs[3] != Peg::None;
            }
            void go_left()
            {
                if (peg > 0)
                    --peg;
            }
            void go_right()
            {
                if (peg < 3)
                    ++peg;
            }

        };

        struct Feedback {
            std::array<Pin, 4> pins {{Pin::None}};

            std::size_t num_pins(Pin p) const 
            {
                std::size_t ret = 0;
                for (auto val : pins)
                    if (val == p) ++ret;
                return ret;
            }
        };

        Game(Board& board) : m_board(board), m_guess(0), m_state(State::None)
        {}

        void start(const std::array<Peg, 4> code)
        {
            m_board.clear();
            m_guess = 0;
            m_state = State::Play;
            m_code  = code;
        }

        template<typename RandomEngine>
        void start(const RandomEngine& rnd)
        {
            m_board.clear();
            m_guess = 0;
            m_state = State::Play;

            // create a random code that the player needs to break.
            for (auto& peg : m_code)
            {
                peg = (Peg)((rnd() % 6) + 1);
            }
        }

        Feedback play(Guess guess)
        {
            const auto max = m_board.size();
            assert(m_guess < max);

            Feedback ret;

            std::size_t pins = 0;

            auto& slot = m_board[m_guess];
            slot.pegs  = guess.pegs;

            auto code = m_code;

            // award black pins
            for (std::size_t i=0; i<4; ++i)
            {
                if (guess.pegs[i] == code[i])
                {
                    ret.pins[pins++] = Pin::Black;
                    guess.pegs[i] = Peg::None;
                    code[i] = Peg::None;
                }
            }
            if (pins == 4)
                m_state = State::Win;            

            // award white pins
            for (std::size_t i=0; i<4; ++i)
            {
                if (guess.pegs[i] == Peg::None)
                    continue;
                for (auto& peg : code)
                {
                    if (peg == Peg::None)
                        continue;
                    if (peg == guess.pegs[i]) 
                    {
                        if (pins < 4)
                            ret.pins[pins++] = Pin::White;
                        peg = Peg::None;
                        break;
                    }
                }
            }
            slot.pins = ret.pins;

            m_guess++;

            if (m_guess == m_board.size())
            {
                if (m_state != State::Win)
                    m_state = State::Loose;
            }
            return ret;
        }

        bool is_running() const
        {
            return m_state == State::Play;
        }

        State get_state() const 
        {
            return m_state;
        }

        const Board& get_board() const 
        {
            return m_board;
        }

        std::size_t max_guesses() const 
        {
            return m_board.size();
        }

        std::size_t cur_guess() const
        {
            return m_guess;
        }
        std::array<Peg, 4> get_code() const 
        { 
            return m_code; 
        }

    private:
        Board& m_board;
    private:
        std::array<Peg, 4> m_code;
        std::size_t m_guess;
    private:
        State m_state;

    };

} // mastermind
