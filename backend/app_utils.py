from typing import Dict, Any
from bottle import load_events
from config import BOTTLE_CAPACITY_ML


def get_water_level() -> Dict[str, Any]:
    """
    Return current water level for the app.
    Safe for polling every ~30s.
    """
    events = load_events()

    if not events:
        return {
            "water_level_ml": 0,
            "capacity_ml": BOTTLE_CAPACITY_ML,
            "ts": None,
        }

    last_event = events[-1]

    return {
        "water_level_ml": last_event["water_level_ml"],
        "capacity_ml": BOTTLE_CAPACITY_ML,
        "ts": last_event["ts"],
    }
