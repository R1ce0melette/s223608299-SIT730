#include "morse.h"
/*
Name: Long Hoang Do
id: s223608299
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);  //9600 bit rate communication
}

void loop() {
  char ch;
  bool done = false;
  while (!done)
  {
    if (Serial.available())
    {
      ch = Serial.read();
      done = dispChar(ch);
    }
  }
  do
  {
    ch = Serial.read();
  }while(ch != 'y'&& ch != 'n');
   
  if (ch != 'y')
  {
    while(true) {}
  }
}
