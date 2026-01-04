# main.py
from flask import Flask, request, jsonify
from flask_cors import CORS
from bottle import (
    save_event,
    validate_event,
    get_settings,
    set_settings,
    build_dashboard
)
from app_utils import get_water_level, get_today_drink_history
from config import FLASK_HOST, FLASK_PORT

app = Flask(__name__)
CORS(
    app,
    resources={r"/api/*": {"origins": "*"}},
)


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

    response = {"status": "ok"}

    return jsonify(response), 200


@app.route("/api/bottle/get-settings", methods=["GET"])
def bottle_get_settings():
    return jsonify(get_settings()), 200


# ---------- App ----------

@app.route("/api/app/water-level", methods=["GET"])
def app_get_water_level():
    return jsonify(get_water_level()), 200


@app.route("/api/app/drink-amount-graph", methods=["GET"])
def app_get_today_drink_history():
    return jsonify(get_today_drink_history()), 200


@app.route("/api/app/set-settings", methods=["POST"])
def app_set_settings():
    data = request.get_json()
    if not data or "mode" not in data or "goal" not in data:
        return jsonify({"error": "Missing mode or goal"}), 400

    try:
        set_settings(
            mode=data["mode"],
            goal=data["goal"]
        )
    except ValueError as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200

@app.route("/api/app/get-settings", methods=["GET"])
def app_get_settings():
    return jsonify(get_settings()), 200


if __name__ == "__main__":
    app.run(host=FLASK_HOST, port=FLASK_PORT, debug=True)
