# led_state.py

class LEDState:
    def __init__(self):
        self.auto_mode = True
        self.manual = [0, 0, 0]  # manual values
        self.auto = [0, 0, 0]    # auto-calculated values

    def get_effective_values(self):
        return self.auto if self.auto_mode else self.manual

    def set_manual(self, values):
        self.manual = values

    def set_auto(self, values):
        self.auto = values

    def set_mode(self, auto_mode):
        self.auto_mode = auto_mode

    def to_dict(self):
        return {
            "mode": "auto" if self.auto_mode else "manual",
            "manual": self.manual,
            "auto": self.auto,
            "effective": self.get_effective_values()
        }

led_state = LEDState()

