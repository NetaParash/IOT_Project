from datetime import datetime, timezone
from bottle import load_events, clear_events
from typing import Dict, Any
from config import BOTTLE_CAPACITY_ML

def get_today_drink_history(bottle_id: int):
    now = datetime.now(timezone.utc)
    today_start = datetime(
        year=now.year,
        month=now.month,
        day=now.day,
        tzinfo=timezone.utc,
    ).timestamp()

    events = load_events(bottle_id)

    chart_points = [
        {
            "x": datetime.fromtimestamp(e["ts"], tz=timezone.utc).isoformat(),
            "y": e["amount_drank_ml"],
        }
        for e in events
        if e.get("amount_drank_ml", 0) > 0 and e["ts"] >= today_start
    ]
    return chart_points


def get_water_level(bottle_id: int) -> Dict[str, Any]:
    events = load_events(bottle_id)

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


def get_total_drank_today(bottle_id: int) -> int:
    events = load_events(bottle_id)

    if not events:
        return 0

    # sum of all drink events today
    now = datetime.now(timezone.utc)
    today_start = datetime(
        year=now.year,
        month=now.month,
        day=now.day,
        tzinfo=timezone.utc,
    ).timestamp()

    return sum(
        e["amount_drank_ml"]
        for e in events
        if e.get("amount_drank_ml", 0) > 0 and e["ts"] >= today_start
    )


def clear_event_data(bottle_id: int):
    clear_events(bottle_id)
