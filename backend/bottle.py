# bottle.py
import json
import time
from typing import Dict, Any
from config import EVENTS_FILE, SETTINGS_FILE, BottleMode


# ---------- Events ----------

def save_event(event: Dict[str, Any]) -> None:
    """
    Append a bottle event to storage.
    """
    with open(EVENTS_FILE, "a") as f:
        f.write(json.dumps(event) + "\n")


def load_events(limit: int | None = None) -> list[dict]:
    """
    Load events from storage.
    """
    if not EVENTS_FILE.exists():
        return []

    with open(EVENTS_FILE) as f:
        lines = f.readlines()

    if limit:
        lines = lines[-limit:]

    return [json.loads(line) for line in lines]


# ---------- Settings ----------

def set_settings(mode: int, goal: int) -> None:
    if mode not in BottleMode._value2member_map_:
        raise ValueError("Invalid mode")

    if not isinstance(goal, int) or goal <= 0:
        raise ValueError("Invalid goal")

    with open(SETTINGS_FILE, "w") as f:
        json.dump({"mode": mode, "goal": goal}, f)


def set_settings(mode: int, goal: int) -> None:
    if mode not in BottleMode._value2member_map_:
        raise ValueError("Invalid mode")

    with open(SETTINGS_FILE, "w") as f:
        json.dump({"mode": mode, "goal": goal}, f)


# ---------- Dashboard ----------

def build_dashboard() -> dict:
    events = load_events()

    if not events:
        return {
            "today_drank_ml": 0,
            "current_water_level_ml": 0,
            "last_update_ts": None,
            "mode": get_settings()["mode"],
        }

    total_drank = sum(e["amount_drank_ml"] for e in events)
    last_event = events[-1]

    return {
        "today_drank_ml": total_drank,
        "current_water_level_ml": last_event["water_level_ml"],
        "last_update_ts": last_event["ts"],
        "mode": get_settings()["mode"],
    }


# ---------- Validation ----------

def validate_event(data: dict) -> dict:
    required = {
        "ts": int,
        "amount_drank_ml": int,
        "water_level_ml": int,
        "mode": int,
    }

    for field, field_type in required.items():
        if field not in data:
            raise ValueError(f"Missing field: {field}")
        if not isinstance(data[field], field_type):
            raise ValueError(f"Invalid type for {field}")

    if data["mode"] not in BottleMode._value2member_map_:
        raise ValueError("Invalid mode enum")

    return data
