#include "Arduino.h"
#include "morse.h"

char *morse_letters[] = {
    // The letters A-Z in Morse code
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char *morse_numbers[] = {
    // The numbers 0-9 in Morse code
    "-----", ".----", "..---", "...--", "....-",
    ".....", "-....", "--...", "---..", "----."};

void blink_dit_or_dah(char dit_or_dah)
{
  digitalWrite(LED_BUILTIN, HIGH);


  // blink dot or dash 
  if (dit_or_dah == '.')
  {
    delay(DOT_TIME);
    Serial.println('.');
  }
  else
  {
    delay(DASH_TIME);
    Serial.println('-');
  }
  digitalWrite(LED_BUILTIN, LOW);

  // delay between the same letter time
  delay(SAME_LETTER_TIME);
}

void blink_morse_code(char *morse_code)
{

  unsigned int i = 0;
  // Read the dots and dashes and blink accordingly
  for(int i(0); morse_code[i] != NULL; i++) blink_dit_or_dah(morse_code[i]);

  // Space between two letters is equal to three dots
  delay(DIFF_LETTER_TIME);
}



bool dispChar(const char ch)                                       // displays the character on the serial monitor as well as on the LED 
{

  if (ch >= 'A' && ch <= 'Z')
  {
    Serial.println(ch);
    blink_morse_code(morse_letters[ch - 'A']);
  }
  // Check for lowercase letters
  else if (ch >= 'a' && ch <= 'z')
  {
    Serial.println(ch);
    blink_morse_code(morse_letters[ch - 'a']);
  }
  // Check for numbers
  else if (ch >= '0' && ch <= '9')
  {
    Serial.println(ch);
    blink_morse_code(morse_numbers[ch - '0']);
  }
  // Check for space between words
  else if (ch == ' ')
  {
    delay(WORD_SPACE);
  }
  // Check for full stop value
  else if (ch == '.')
  {
    return true;
  }
  return false;
}
