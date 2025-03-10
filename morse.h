#ifndef MORSE_H
#define MORSE_H

#define UNIT 300
#define DOT_TIME UNIT          // time for a dot
#define DASH_TIME 3*UNIT         // time for a dash
#define WORD_SPACE 7*UNIT      // space between two words - 7 units
#define SAME_LETTER_TIME UNIT  // delay between symbols of the same charater = 1 unit
#define DIFF_LETTER_TIME 3*UNIT  // delay between 2 characters = 3 unit


bool dispChar(char ch);


#endif