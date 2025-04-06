import tkinter as tk
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
LED_PINS = {
    'Green': 18,
    'Red': 13,
    'Yellow': 12
}

# Pin definitions
GPIO.setup(LED_PINS['Green'], GPIO.OUT)
GPIO.setup(LED_PINS['Red'], GPIO.OUT)
GPIO.setup(LED_PINS['Yellow'], GPIO.OUT)

green_pwm = GPIO.PWM(LED_PINS['Green'], 100)
red_pwm = GPIO.PWM(LED_PINS['Red'], 100)
yellow_pwm = GPIO.PWM(LED_PINS['Yellow'], 100)

green_pwm.start(0)
red_pwm.start(0)
yellow_pwm.start(0)

root = tk.Tk()
root.title("PWM LED Controller")
root.geometry("320x450")

# Sliders
tk.Label(root, text="Green LED Intensity", fg='green').pack()
green_slider = tk.Scale(root, from_=0, to=100, orient=tk.HORIZONTAL)
green_slider.pack()

tk.Label(root, text="Red LED Intensity", fg='red').pack()
red_slider = tk.Scale(root, from_=0, to=100, orient=tk.HORIZONTAL)
red_slider.pack()

tk.Label(root, text="Yellow LED Intensity", fg='gold').pack()
yellow_slider = tk.Scale(root, from_=0, to=100, orient=tk.HORIZONTAL)
yellow_slider.pack()

# change intensity
def update_leds():
    green_pwm.ChangeDutyCycle(green_slider.get())
    red_pwm.ChangeDutyCycle(red_slider.get())
    yellow_pwm.ChangeDutyCycle(yellow_slider.get())
    # Timer: 100 milliseconds
    root.after(100, update_leds)

def start_timer():
    update_leds()

start_button = tk.Button(root, text="Start", command=start_timer)
start_button.pack(pady=10)

# Run the GUI
try:
    root.mainloop()
finally:
    # Clean up GPIO on exit
    green_pwm.stop()
    red_pwm.stop()
    yellow_pwm.stop()
    GPIO.cleanup()