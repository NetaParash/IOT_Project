from datetime import datetime, timezone
from bottle import load_events
from typing import Dict, Any
from config import BOTTLE_CAPACITY_ML


def get_today_drink_history():
    now = datetime.now(timezone.utc)
    today_start = datetime(
        year=now.year,
        month=now.month,
        day=now.day,
        tzinfo=timezone.utc,
    ).timestamp()

    events = load_events()

    chart_points = [
        {
            "x": datetime.fromtimestamp(e["ts"], tz=timezone.utc).isoformat(),
            "y": e["amount_drank_ml"],
        }
        for e in events
        if e.get("amount_drank_ml", 0) > 0 and e["ts"] >= today_start
    ]
    print(chart_points)
    return chart_points


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


def get_total_drank_today():
    events = load_events()

    if not events:
        return {
            "water_level_ml": 0,
            "capacity_ml": BOTTLE_CAPACITY_ML,
            "ts": None,
        }

    last_event = events[-1]

    return last_event["amount_drank_ml"]
