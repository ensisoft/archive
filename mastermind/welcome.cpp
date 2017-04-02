
#include "welcome.h"
#include "buffer.h"
#include "formatter.h"
#include "cursor.h"

namespace mastermind
{

void Welcome::draw(Buffer& buff, Cursor& cursor)
{
    Formatter fmt(buff);

    fmt.align(Formatter::Align::Center);
    fmt.set(Hilites::Bold, true);
    fmt.set(Hilites::Underline, true);
    fmt.print(3, "Mastermind The Game");
    fmt.set(Hilites::Underline, false);    
    fmt.set(Hilites::Bold, false);        
    fmt.print(5, "Break the secret code!");
    fmt.print(7, "You have 6 different pegs of different color.");
    fmt.print(8, "Guess the correct code by choosing 4 different pegs.");
    fmt.print(9, "For each guess you'll receive feedback pins.");
    fmt.print(10, "Black (b) pin means a correct peg at a correct place.");
    fmt.print(11, "White (w) pin means a correct peg but at incorrect place.");
    fmt.print(12, "You have 12 attempts to break the code.");
    fmt.print(14, "Good luck.");

    fmt.set(Hilites::Blink, true);
    fmt.set(Hilites::Bold, true);
    fmt.print(16, "Press 'N' for a New Game");
    fmt.set(Hilites::Blink, false);    
    fmt.print(18, "Press 'Q' to Quit");

    cursor.hide();
}

void Welcome::react(Keysym sym) 
{
    // nothing here.
}

} // mastermind