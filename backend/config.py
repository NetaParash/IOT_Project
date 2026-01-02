# config.py
from enum import IntEnum
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent
DATA_DIR = BASE_DIR / "data"
DATA_DIR.mkdir(exist_ok=True)

EVENTS_FILE = DATA_DIR / "bottle_events.jsonl"
SETTINGS_FILE = DATA_DIR / "settings.json"


class BottleMode(IntEnum):
    NORMAL = 0
    ECO = 1
    SPORT = 2
    DEBUG = 3


FLASK_HOST = "0.0.0.0"
FLASK_PORT = 5003
