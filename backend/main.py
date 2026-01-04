# main.py
from flask import Flask, request, jsonify
from flask_cors import CORS
from bottle import (
    save_event,
    validate_event,
    get_settings,
    set_settings,
)
from app_utils import get_water_level, get_today_drink_history, get_total_drank_today, clear_event_data
from config import FLASK_HOST, FLASK_PORT

app = Flask(__name__)
CORS(app, resources={r"/api/*": {"origins": "*"}})

# ---------- Bottle ----------

@app.route("/api/bottle/events", methods=["POST"])
def bottle_post_event():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        event = validate_event(data)
        save_event(event)
    except ValueError as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200

@app.route("/api/bottle/settings", methods=["GET"])
def bottle_get_settings():
    return jsonify(get_settings()), 200


@app.route("/api/bottle/settings", methods=["POST"])
def bottle_set_settings():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        set_settings(
            mode=data["mode"],
            goal=data["goal"],
            alerts_every=data["alerts_every"],
        )
    except (KeyError, ValueError) as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200

@app.route("/api/app/settings", methods=["GET"])
def app_get_settings():
    return jsonify(get_settings()), 200


@app.route("/api/app/settings", methods=["POST"])
def app_set_settings():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        set_settings(
            mode=data["mode"],
            goal=data["goal"],
            alerts_every=data["alerts_every"],
        )
    except (KeyError, ValueError) as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200


@app.route("/api/app/water-level", methods=["GET"])
def app_get_water_level():
    return jsonify(get_water_level()), 200

@app.route("/api/app/total-drank-today", methods=["GET"])
def app_total_drank_today():
    return jsonify({"total_drank_today_ml":get_total_drank_today()}), 200


@app.route("/api/app/drink-amount-graph", methods=["GET"])
def app_get_today_drink_history():
    return jsonify(get_today_drink_history()), 200

@app.route("/api/app/clear-event-data", methods=["POST"])
def delete_event_data():
    """
    Delete all bottle event data (used for reset / testing).
    """
    try:
        clear_event_data()

        return jsonify({
            "status": "ok",
            "message": "All event data deleted"
        }), 200

    except Exception as e:
        return jsonify({
            "status": "error",
            "message": str(e)
        }), 500


if __name__ == "__main__":
    app.run(host=FLASK_HOST, port=FLASK_PORT, debug=True)
