
#include "test_minimal.h"
#include "../game.h"

void unit_test_game()
{
    mastermind::Board board(12);
    mastermind::Game game(board);

    using Peg = mastermind::Peg;
    using Pin = mastermind::Pin;

    {
        std::array<mastermind::Peg, 4> code = {
            Peg::Red,
            Peg::Red,
            Peg::White,
            Peg::White
        };
        game.start(code);

        mastermind::Game::Guess guess;
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);        
        guess.add_peg(Peg::Green);        

        auto pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 0);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 0);        

        guess.clear();
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);                
        guess.add_peg(Peg::Red);

        pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 1);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 0);                

        guess.clear();
        guess.add_peg(Peg::Red);
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);                
        guess.add_peg(Peg::Green);        
        pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 0);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 1);                        

        guess.clear();
        guess.add_peg(Peg::Red);
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Red);                
        guess.add_peg(Peg::Green);        
        pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 1);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 1);                        

        guess.clear();
        guess.add_peg(Peg::Red);
        guess.add_peg(Peg::Red);                        
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Green);        
        pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 0);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 2);                                

    }

    {
        std::array<mastermind::Peg, 4> code = {
            Peg::Red,
            Peg::Orange,
            Peg::Orange,
            Peg::Green
        };
        game.start(code);        

        mastermind::Game::Guess guess;

        guess.clear();
        guess.add_peg(Peg::Orange);
        guess.add_peg(Peg::Blue);                        
        guess.add_peg(Peg::Green);
        guess.add_peg(Peg::Red);        
        auto pins = game.play(guess);
        TEST_REQUIRE(pins.num_pins(Pin::White) == 3);
        TEST_REQUIRE(pins.num_pins(Pin::Black) == 0);                                        
    }

}

int test_main(int argc, char* argv[])
{
    unit_test_game();
    return 0;
}