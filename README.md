## Multiple interrupt demonstration with 3 LED setup and timer interrupt.
# Wiring
•	Blue wire connects pin 10 to central terminal of the SPDT slide switch 
•	Brown wire connects pin 9 to anode of red LED 
•	Red wire connects pin 8 to anode of blue LED 
•	Yellow wire connects pin 7 to anode of yellow LED 
•	220 Ohm resistors connects cathode of blue LED to the top negative power line 
•	220 Ohm resistors connects cathode of red LED to the bottom negative power line 
•	220 Ohm resistors connects cathode of yellow LED to the bottom negative power line 
•	Pale Brown wire connects pin 6 to data pin of DHT11 
•	Black wire connects arduino GND pin to bottom negative power line 
•	Second black wire connects arduino other GND pin to top negative power line 
•	White wire connects right terminal of the SPDT slide switch to top negative power line 
•	Orange wire connects 3.3V pin to left terminal of the SPDT slide switch 
•	Grey wire connects 3.3V pin to bottom positive power line 
•	Green wire connects GND pin of DHT11 to bottom negative power line 
•	Red wire connects VCC pin of DHT11 to bottom positive power line
>
# Hardware
•	Arduino Nano 33 IoT
•	3 LEDs
•	SPDT slide switch
•	3x 220 Ohm resistors
•	Temperature and humidity sensor DHT11
# Library
•	DHT11
•	SAMD_TimerInterrupt
