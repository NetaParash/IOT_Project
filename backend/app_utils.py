from datetime import datetime, timezone
from bottle import load_events


def get_today_drink_history():
    """
    Return drink events from today only.
    """
    now = datetime.now(timezone.utc)
    today_start = datetime(
        year=now.year,
        month=now.month,
        day=now.day,
        tzinfo=timezone.utc,
    ).timestamp()

    events = load_events()

    today_events = [
        {
            "ts": e["ts"],
            "amount_ml": e["amount_drank_ml"],
        }
        for e in events
        if e.get("amount_drank_ml", 0) > 0 and e["ts"] >= today_start
    ]

    return today_events



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
