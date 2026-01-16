# config.py
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
DATA_DIR.mkdir(exist_ok=True)

SETTINGS_FILE = DATA_DIR / "settings.json"

BOTTLE_CAPACITY_ML = 500

# -----------------------
# Bottle / App modes
# -----------------------

MODE_HYDRATION = "hydration"
MODE_SPORT = "sport"
MODE_OFFICE = "office"
MODE_NIGHT = "night"
MODE_CUSTOM = "custom"

VALID_MODES = {
    MODE_HYDRATION,
    MODE_SPORT,
    MODE_OFFICE,
    MODE_NIGHT,
    MODE_CUSTOM,
}

# Optional: default presets (mirrors frontend)
MODE_PRESETS = {
    MODE_HYDRATION: {
        "goal": 2500,
        "alerts_every": 60,
    },
    MODE_SPORT: {
        "goal": 3500,
        "alerts_every": 30,
    },
    MODE_OFFICE: {
        "goal": 2000,
        "alerts_every": 90,
    },
    MODE_NIGHT: {
        "goal": 500,
        "alerts_every": 0,
    },
}

DEFAULT_MODE = MODE_HYDRATION

FLASK_HOST = "0.0.0.0"
FLASK_PORT = 5003
