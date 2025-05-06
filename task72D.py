from vosk import Model, KaldiRecognizer
import pyaudio
import json
import os
import RPi.GPIO as GPIO
import tkinter as tk
from threading import Thread

# catch shutdown_event
shutdown_event = threading.Event()

# declare gpio
LED_PIN = 19
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)
GPIO.output(LED_PIN, GPIO.LOW)

# gui setup
class LEDGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("LED Status")
        self.status_label = tk.Label(root, text="LED is OFF", font=("Arial", 24), fg="red")
        self.status_label.pack(pady=30, padx=30)

    def update_status(self, is_on):
        if is_on:
            self.status_label.config(text="LED is ON", fg="green")
        else:
            self.status_label.config(text="LED is OFF", fg="red")

# speech to text thread
def speech_listener(gui):
    model_path = "vosk-model-small-en-us-0.15"
    if not os.path.exists(model_path):
        print("Model not found!")
        return

    model = Model(model_path)
    recognizer = KaldiRecognizer(model, 16000)

    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000,
                    input=True, frames_per_buffer=8192)
    stream.start_stream()

    print("Say ON or OFF to control LED")

    try:
        while True:
            data = stream.read(4096, exception_on_overflow=False)
            if recognizer.AcceptWaveform(data):
                result = json.loads(recognizer.Result())
                text = result.get("text", "").lower()
                print("Recognized:", text)

                if "on" in text:
                    GPIO.output(LED_PIN, GPIO.HIGH)
                    gui.update_status(True)
                elif "off" in text:
                    GPIO.output(LED_PIN, GPIO.LOW)
                    gui.update_status(False)
    except Exception as e:
        print("Error:", e)
    finally:
        stream.stop_stream()
        stream.close()
        p.terminate()
        GPIO.cleanup()

if __name__ == "__main__":
    root = tk.Tk()
    gui = LEDGUI(root)

    # speech recognition in a background thread
    thread = Thread(target=speech_listener, args=(gui,), daemon=True)
    thread.start()

    # GUI main loop
    try:
        root.mainloop()
    finally:
        GPIO.cleanup()
