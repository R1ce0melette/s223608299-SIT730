const socket = io();

window.onload = () => {
    fetch('/state')
        .then(res => res.json())
        .then(applyStateToUI);

    socket.on('led_update', (data) => {
        applyStateToUI(data);
    });

    document.getElementById("autoToggle").addEventListener("change", function () {
        socket.emit("toggle_auto", this.checked);
    });

    // Only apply manual update if not in auto mode
    ["led1", "led2", "led3"].forEach((id, idx) => {
        document.getElementById(id).addEventListener("input", () => {
            if (!document.getElementById("autoToggle").checked) {
                const values = [
                    parseInt(document.getElementById("led1").value),
                    parseInt(document.getElementById("led2").value),
                    parseInt(document.getElementById("led3").value),
                ];
                socket.emit("manual_update", values);
            }
        });
    });
};

function applyStateToUI(data) {
    const isAuto = data.mode === "auto";
    const values = data.effective;

    ["led1", "led2", "led3"].forEach((id, i) => {
        const el = document.getElementById(id);
        el.value = values[i];
        el.disabled = isAuto;
        updateSliderBackground(id, values[i]);
    });

    ["toggle1", "toggle2", "toggle3"].forEach(id => {
        document.getElementById(id).disabled = isAuto;
    });

    document.getElementById("autoToggle").checked = isAuto;
}

function updateSliderBackground(id, value) {
    const percent = (value / 255) * 100;
    document.getElementById(id).style.background =
        `linear-gradient(to right, #ffd700 ${percent}%, #444 ${percent}%)`;
}

