import time
import asyncio
from smbus import SMBus
from bleak import BleakClient

# === I2C Config ===
DEVICE = 0x23
ONE_TIME_HIGH_RES_MODE = 0x20
bus = SMBus(1)

# === BLE Config ===
ARDUINO_ADDRESS = "B0:B2:1C:56:0F:02"  # arduino ble mac
CHARACTERISTIC_UUID = "19b10010-e8f2-537e-4f6c-d104768a1214"  # Custom UUID

def read_lux():
    data = bus.read_i2c_block_data(DEVICE, ONE_TIME_HIGH_RES_MODE)
    lux = data[1]
    return lux

async def send_lux():
    while True:
        try:
            print("Attempting to connect to Arduino...")
            async with BleakClient(ARDUINO_ADDRESS) as client:
                print("Connected to Arduino.")
                while True:
                    lux = read_lux()
                    print(f"Lux: {lux:.2f}")
                    # Map lux to alert level (0–255), lower lux = higher alert
                    lux_clamped = min(max(0, lux), 120)
                    alert_level = 255 - int((lux_clamped / 120) * 255)
                    await client.write_gatt_char(CHARACTERISTIC_UUID, bytearray([alert_level]))
                    time.sleep(1)
        except Exception as e:
            print(f"Connection failed or lost: {e}")
            print("Retrying in 3 seconds...")
            time.sleep(3)

asyncio.run(send_lux())
