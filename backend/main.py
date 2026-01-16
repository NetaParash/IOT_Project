# main.py
from flask import Flask, request, jsonify
from flask_cors import CORS
from bottle import (
    save_event,
    get_settings,
    set_settings,
)
from app_utils import (
    get_water_level,
    get_today_drink_history,
    get_total_drank_today,
    clear_event_data,
)
from config import FLASK_HOST, FLASK_PORT
import time

app = Flask(__name__)
CORS(app, resources={r"/api/*": {"origins": "*"}})

# ---------- Bottle ----------

@app.route("/api/bottle/<int:bottle_id>/events", methods=["POST"])
def bottle_post_event(bottle_id):
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    event = {
        "bottle_id": bottle_id,
        "ts": int(time.time()),
        "amount_drank_ml": data["amount_drank_ml"],
        "water_level_ml": data["water_level_ml"],
    }

    save_event(event, bottle_id=bottle_id)
    return jsonify({"status": "ok"}), 200


@app.route("/api/bottle/<int:bottle_id>/settings", methods=["GET"])
def bottle_get_settings(bottle_id):
    return jsonify(get_settings(bottle_id=bottle_id)), 200


@app.route("/api/bottle/<int:bottle_id>/settings", methods=["POST"])
def bottle_set_settings(bottle_id):
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        set_settings(
            bottle_id=bottle_id,
            mode=data["mode"],
            goal=data["goal"],
            alerts_every=data["alerts_every"],
        )
    except (KeyError, ValueError) as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200


# ---------- App (Bottle-specific views) ----------

@app.route("/api/app/<int:bottle_id>/settings", methods=["GET"])
def app_get_settings(bottle_id):
    return jsonify(get_settings(bottle_id=bottle_id)), 200


@app.route("/api/app/<int:bottle_id>/settings", methods=["POST"])
def app_set_settings(bottle_id):
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        set_settings(
            bottle_id=bottle_id,
            mode=data["mode"],
            goal=data["goal"],
            alerts_every=data["alerts_every"],
        )
    except (KeyError, ValueError) as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200


@app.route("/api/app/<int:bottle_id>/water-level", methods=["GET"])
def app_get_water_level(bottle_id):
    return jsonify(get_water_level(bottle_id=bottle_id)), 200


@app.route("/api/app/<int:bottle_id>/total-drank-today", methods=["GET"])
def app_total_drank_today(bottle_id):
    return jsonify({
        "total_drank_today_ml": get_total_drank_today(bottle_id=bottle_id)
    }), 200


@app.route("/api/app/<int:bottle_id>/drink-amount-graph", methods=["GET"])
def app_get_today_drink_history(bottle_id):
    return jsonify(get_today_drink_history(bottle_id=bottle_id)), 200


@app.route("/api/app/<int:bottle_id>/clear-event-data", methods=["POST"])
def delete_event_data(bottle_id):
    """
    Delete all bottle event data (used for reset / testing).
    """
    try:
        clear_event_data(bottle_id=bottle_id)

        return jsonify({
            "status": "ok",
            "message": f"All event data deleted for bottle {bottle_id}"
        }), 200

    except Exception as e:
        return jsonify({
            "status": "error",
            "message": str(e)
        }), 500


if __name__ == "__main__":
    app.run(host=FLASK_HOST, port=FLASK_PORT, debug=True)
