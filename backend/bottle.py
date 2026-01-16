# bottle.py
import json
from typing import Dict, Any
from pathlib import Path

from config import (
    DATA_DIR,
    SETTINGS_FILE,
    VALID_MODES,
    DEFAULT_MODE,
    BOTTLE_CAPACITY_ML,
)

# ----------------------------------------
# EVENTS (per bottle)
# ----------------------------------------

def _events_file_for(bottle_id: int) -> Path:
    return DATA_DIR / f"bottle_events_{bottle_id}.jsonl"


def save_event(event: Dict[str, Any], bottle_id: int) -> None:
    fpath = _events_file_for(bottle_id)
    with open(fpath, "a") as f:
        f.write(json.dumps(event) + "\n")


def load_events(bottle_id: int, limit: int | None = None) -> list[dict]:
    fpath = _events_file_for(bottle_id)
    if not fpath.exists():
        return []

    with open(fpath) as f:
        lines = f.readlines()

    if limit:
        lines = lines[-limit:]

    return [json.loads(line) for line in lines]


def clear_events(bottle_id: int) -> None:
    fpath = _events_file_for(bottle_id)
    open(fpath, "w").close()


# ----------------------------------------
# SETTINGS (dict-of-dicts)
# ----------------------------------------

DEFAULT_SETTINGS = {
    "mode": DEFAULT_MODE,
    "goal": 2500,
    "alerts_every": 60,
}

def _load_all_settings() -> dict:
    if not SETTINGS_FILE.exists():
        return {}
    with open(SETTINGS_FILE) as f:
        return json.load(f)


def _write_all_settings(all_settings: dict) -> None:
    with open(SETTINGS_FILE, "w") as f:
        json.dump(all_settings, f, indent=2)


def get_settings(bottle_id: int) -> dict:
    all_settings = _load_all_settings()
    return all_settings.get(str(bottle_id), DEFAULT_SETTINGS.copy())


def set_settings(bottle_id: int, mode: str, goal: int, alerts_every: int) -> None:
    if mode not in VALID_MODES:
        raise ValueError("Invalid mode")

    if not isinstance(goal, int) or goal <= 0:
        raise ValueError("Invalid goal")

    if not isinstance(alerts_every, int) or alerts_every < 0:
        raise ValueError("Invalid alerts_every")

    all_settings = _load_all_settings()
    all_settings[str(bottle_id)] = {
        "mode": mode,
        "goal": goal,
        "alerts_every": alerts_every,
    }

    _write_all_settings(all_settings)
