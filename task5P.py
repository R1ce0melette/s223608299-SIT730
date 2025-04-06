import tkinter as tk
import RPi.GPIO as GPIO

# Pin definitions
LED_PINS = {
    'Green': 5,
    'Yellow': 6,
    'Red': 13
}
GPIO.setmode(GPIO.BCM)
for pin in LED_PINS.values():
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)

def turn_on_led(led_color):
    # turn leds off
    GPIO.output(5, GPIO.LOW)
    GPIO.output(6, GPIO.LOW)
    GPIO.output(13, GPIO.LOW)

    # turn on the selected LED
    if led_color == 'Green':
        GPIO.output(5, GPIO.HIGH)
    elif led_color == 'Yellow':
        GPIO.output(6, GPIO.HIGH)
    elif led_color == 'Red':
        GPIO.output(13, GPIO.HIGH)
def turn_off_all(): #for button
    GPIO.output(5, GPIO.LOW)
    GPIO.output(6, GPIO.LOW)
    GPIO.output(13, GPIO.LOW)
    led_var.set(None)
def execute_action():
    user_input = user_input_box.get().strip().lower()  # process input

    if user_input == 'green':
        turn_on_led('Green')
    elif user_input == 'yellow':
        turn_on_led('Yellow')
    elif user_input == 'red':
        turn_on_led('Red')
    else:
        turn_off_all()
#init window
root = tk.Tk()
root.title("LED Controller")
root.geometry("320x320")

#led state
led_var =tk.StringVar()
#init buttons
green_radio = tk.Radiobutton(root, text="Green LED", variable=led_var, value='Green',
                             command=lambda: turn_on_led('Green'), fg='green')
green_radio.pack()

yellow_radio = tk.Radiobutton(root, text="Yellow LED", variable=led_var, value='Yellow',
                              command=lambda: turn_on_led('Yellow'), fg='yellow')
yellow_radio.pack()

red_radio = tk.Radiobutton(root, text="Red LED", variable=led_var, value='Red',
                           command=lambda: turn_on_led('Red'), fg='red')
red_radio.pack()

# set label for text box
input_label = tk.Label(root, text="Enter LED color (green, yellow, red):")
input_label.pack()

# text box
user_input_box = tk.Entry(root)
user_input_box.pack()

# execute button
execute_button = tk.Button(root, text="Execute", command=execute_action)
execute_button.pack()

root.mainloop()
GPIO.cleanup()