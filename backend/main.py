# main.py
from flask import Flask, request, jsonify
from bottle import (
    save_event,
    validate_event,
    get_settings,
    set_settings,
    build_dashboard
)
from config import FLASK_HOST, FLASK_PORT

app = Flask(__name__)


# ---------- Bottle ----------

@app.route("/api/bottle/events", methods=["POST"])
def bottle_post_event():
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON received"}), 400

    try:
        event = validate_event(data)
        save_event(event)
        set_settings(event["mode"])
    except ValueError as e:
        return jsonify({"error": str(e)}), 400

    response = {"status": "ok"}

    return jsonify(response), 200


@app.route("/api/bottle/settings", methods=["GET"])
def bottle_get_settings():
    return jsonify(get_settings()), 200


# ---------- App ----------

@app.route("/api/app/dashboard", methods=["GET"])
def app_get_dashboard():
    return jsonify(build_dashboard()), 200


@app.route("/api/app/settings", methods=["POST"])
def app_set_settings():
    data = request.get_json()
    if not data or "mode" not in data:
        return jsonify({"error": "Missing mode"}), 400

    try:
        set_settings(data["mode"])
    except ValueError as e:
        return jsonify({"error": str(e)}), 400

    return jsonify({"status": "ok"}), 200


if __name__ == "__main__":
    app.run(host=FLASK_HOST, port=FLASK_PORT, debug=True)
