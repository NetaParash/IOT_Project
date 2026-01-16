import json
import random
from datetime import datetime, timedelta
from pathlib import Path

# -----------------------------
# CONFIG
# -----------------------------
DATA_DIR = Path("data")
DATA_DIR.mkdir(exist_ok=True)

GOAL = 2500

COMPLETED = [2, 5, 9]
EMPTY = [3, 6, 11, 14, 18]
ALL = list(range(2, 21))
RANDOM_BOTTLES = [b for b in ALL if b not in COMPLETED + EMPTY]


# -----------------------------
# HELPERS
# -----------------------------
def write_events(bottle_id, events):
    fpath = DATA_DIR / f"bottle_events_{bottle_id}.jsonl"
    with open(fpath, "w") as f:
        for event in events:
            f.write(json.dumps(event) + "\n")
    print(f"✔ Created {fpath} ({len(events)} events)")


def ts(offset_minutes):
    now = datetime.now().replace(hour=8, minute=0, second=0, microsecond=0)
    return int((now + timedelta(minutes=offset_minutes)).timestamp())


def start_event():
    """Initial zero-drink event."""
    return {
        "ts": ts(0),
        "amount_drank_ml": 0,
        "water_level_ml": random.randint(0, 500),
    }


# -----------------------------
# 1. COMPLETED GOAL (0 → 2500)
# -----------------------------
def generate_completed():
    sips = [300, 500, 700, 1000]  # total = 2500
    events = [start_event()]

    t = 0
    total = 0

    for sip in sips:
        t += random.randint(45, 90)
        total += sip

        events.append({
            "ts": ts(t),
            "amount_drank_ml": total,
            "water_level_ml": random.randint(0, 500),
        })

    return events


# -----------------------------
# 2. EMPTY FILES
# -----------------------------
def generate_empty():
    return []


# -----------------------------
# 3. RANDOM (0 → random ≤ 2300)
# -----------------------------
def generate_random():
    target = random.randint(300, 2300)
    events = [start_event()]

    t = 0
    total = 0

    while total < target:
        sip = min(
            target - total,
            random.choice([100, 150, 200, 250, 300, 400, 500]),
        )

        t += random.randint(30, 90)
        total += sip

        events.append({
            "ts": ts(t),
            "amount_drank_ml": total,
            "water_level_ml": random.randint(0, 500),
        })

    return events


# -----------------------------
# MAIN
# -----------------------------
def generate_mock_events():
    for b in COMPLETED:
        write_events(b, generate_completed())

    for b in EMPTY:
        write_events(b, generate_empty())

    for b in RANDOM_BOTTLES:
        write_events(b, generate_random())

    print("\n🎉 Finished generating mock bottle events!\n")

