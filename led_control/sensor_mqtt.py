# sensor_mqtt.py

from smbus2 import SMBus
import time
import paho.mqtt.client as mqtt
from led_state import led_state

BH1750_ADDR = 0x23  # Default address for BH1750
BH1750_CMD = 0x10   # Continuously H-Resolution Mode

MQTT_BROKER = "localhost"
MQTT_TOPIC = "led/control"

def read_lux(bus):
    try:
        data = bus.read_i2c_block_data(BH1750_ADDR, BH1750_CMD, 2)
        lux = (data[0] << 8 | data[1]) / 1.2
        return min(max(lux, 0), 2000)  # Clamp to [0, 2000]
    except Exception as e:
        print("BH1750 read error:", e)
        return 0

def lux_to_pwm(lux):
    brightness = int((1 - (lux / 2000.0)) * 255)
    return max(0, min(brightness, 255))

def start_sensor_loop():
    bus = SMBus(1)  # Use I2C bus 1
    client = mqtt.Client()
    client.connect(MQTT_BROKER)

    while True:
        lux = read_lux(bus)
        brightness = lux_to_pwm(lux)

        # Apply same brightness to all LEDs (or vary per LED if desired)
        auto_values = [brightness, brightness, brightness]
        led_state.set_auto(auto_values)

        # Publish to Arduino via MQTT
        if led_state.auto_mode:
            payload = {
              "led1": auto_values[0],
              "led2": auto_values[1],
              "led3": auto_values[2]
            }
            client.publish(MQTT_TOPIC, str(payload))
            print(f"Lux: {lux:.1f} → Brightness: {auto_values} → MQTT published")
        time.sleep(10)

