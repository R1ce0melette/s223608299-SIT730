# app.py

from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO
from led_state import led_state
import threading
import time
from sensor_mqtt import start_sensor_loop
import paho.mqtt.publish as publish
import json

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode='eventlet')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/state')
def get_state():
    return jsonify(led_state.to_dict())

@socketio.on("manual_update")
def handle_manual_update(values):
    led_state.set_manual(values)
    if not led_state.auto_mode:
        payload = json.dumps({"led1": values[0], "led2": values[1], "led3": values[2]})
        publish.single("led/control", payload=payload, hostname="192.168.0.253")

@socketio.on("toggle_auto")
def handle_toggle_auto(enabled):
    led_state.set_mode(enabled)
    socketio.emit("led_update", led_state.to_dict())

@app.route('/set_manual', methods=['POST'])
def set_manual():
    data = request.json
    values = [data.get('led1', 0), data.get('led2', 0), data.get('led3', 0)]
    led_state.set_manual(values)
    if not led_state.auto_mode:
        payload = json.dumps({"led1": values[0], "led2": values[1], "led3": values[2]})
        publish.single("led/control", payload=payload, hostname="192.168.0.253")
    return jsonify({"status": "ok", "values": values})

@app.route('/set_mode', methods=['POST'])
def set_mode():
    data = request.json
    led_state.set_mode(data.get('auto_mode', True))
    return jsonify({"status": "ok", "mode": led_state.auto_mode})

# Background thread: push auto values every 10s
def auto_update_loop():
    while True:
        if led_state.auto_mode:
            print("[WS] Emitting to WebSocket:", led_state.get_effective_values()) 
            socketio.emit('led_update', led_state.to_dict())
        time.sleep(10)

# Start background thread
threading.Thread(target=auto_update_loop, daemon=True).start()
threading.Thread(target=start_sensor_loop, daemon=True).start()

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)

