##Code repo for Task 1.1P - SIT730 Embedded System

Blink.ino is the bare minimum code for this task. It's hardcoded program that makes the built in LED on Arduino Nano 33 IoT blink my name - Long in morse code.

Task1.ino is the modularized version, utilizing C++ for the logic. I use an array to map alphabet characters to their morse code.
Time delay is self-explained in header file - morse.h
Task1 code recursively wait for user input to display to serial monitor and blink the led.

