#!/bin/bash

set -e  # Exit immediately if any command fails

# Resolve the script's directory
APP_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
VENV_DIR="$APP_DIR/venv"
SERVICE_NAME="ledcontrol.service"
SYSTEMD_PATH="/etc/systemd/system"

echo "=== Smart LED Control Installer ==="
echo "Project directory: $APP_DIR"

# Step 1: Create virtual environment
if [ ! -d "$VENV_DIR" ]; then
    echo "[1/5] Creating virtual environment..."
    python3 -m venv "$VENV_DIR"
else
    echo "[1/5] Virtual environment already exists. Skipping..."
fi

# Step 2: Install Python dependencies
echo "[2/5] Installing Python dependencies..."
source "$VENV_DIR/bin/activate"
if [ -f "$APP_DIR/requirements.txt" ]; then
    pip install --upgrade pip
    pip install -r "$APP_DIR/requirements.txt"
else
    echo "Error: requirements.txt not found in $APP_DIR"
    exit 1
fi
deactivate

# Step 3: Prepare systemd service file (edit dynamically)
SERVICE_PATH="$APP_DIR/$SERVICE_NAME"

if [ ! -f "$SERVICE_PATH" ]; then
    echo "Error: $SERVICE_NAME not found in $APP_DIR"
    exit 1
fi

echo "[3/5] Configuring systemd service..."
TEMP_SERVICE=$(mktemp)
sed "s|WorkingDirectory=.*|WorkingDirectory=$APP_DIR|" "$SERVICE_PATH" | \
sed "s|ExecStart=.*|ExecStart=$VENV_DIR/bin/python $APP_DIR/app.py|" | \
sed "s|Environment=.*|Environment=PATH=$VENV_DIR/bin|" > "$TEMP_SERVICE"

# Step 4: Copy to systemd and enable
echo "[4/5] Installing systemd service..."
sudo cp "$TEMP_SERVICE" "$SYSTEMD_PATH/$SERVICE_NAME"
sudo systemctl daemon-reload
sudo systemctl enable "$SERVICE_NAME"
sudo systemctl restart "$SERVICE_NAME"

# Step 5: Show status
echo "[5/5] Service status:"
sudo systemctl status "$SERVICE_NAME" --no-pager

# Cleanup
rm "$TEMP_SERVICE"

echo "Installation complete."
