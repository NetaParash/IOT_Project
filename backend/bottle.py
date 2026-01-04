# bottle.py
import json
from typing import Dict, Any
from config import EVENTS_FILE, SETTINGS_FILE, BottleMode

# ---------- Events ----------

def save_event(event: Dict[str, Any]) -> None:
    with open(EVENTS_FILE, "a") as f:
        f.write(json.dumps(event) + "\n")


def load_events(limit: int | None = None) -> list[dict]:
    if not EVENTS_FILE.exists():
        return []

    with open(EVENTS_FILE) as f:
        lines = f.readlines()

    if limit:
        lines = lines[-limit:]

    return [json.loads(line) for line in lines]


def validate_event(data: dict) -> dict:
    required = {
        "ts": int,
        "amount_drank_ml":int,
        "water_level_ml": int,
    }

    for field, t in required.items():
        if field not in data or not isinstance(data[field], t):
            raise ValueError(f"Invalid or missing field: {field}")

    return data


# ---------- Settings ----------

DEFAULT_SETTINGS = {
    "mode": BottleMode.NORMAL,
    "goal": 1500,
    "alerts_every": 30,
}


def get_settings() -> dict:
    if not SETTINGS_FILE.exists():
        return DEFAULT_SETTINGS.copy()

    with open(SETTINGS_FILE) as f:
        return json.load(f)


def set_settings(mode: int, goal: int, alerts_every: int) -> None:
    if mode not in BottleMode._value2member_map_:
        raise ValueError("Invalid mode")

    if not isinstance(goal, int) or goal <= 0:
        raise ValueError("Invalid goal")

    if not isinstance(alerts_every, int) or alerts_every <= 0:
        raise ValueError("Invalid alerts_every")

    with open(SETTINGS_FILE, "w") as f:
        json.dump(
            {
                "mode": mode,
                "goal": goal,
                "alerts_every": alerts_every,
            },
            f,
        )
