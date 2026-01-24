# 🧴 SmartBottle Backend API
A lightweight Flask backend for handling hydration bottle events, settings, analytics, and mock data generation.

This backend powers the **SmartBottle IoT project**, where ESP32-based bottles send drinking events, and a React dashboard visualizes progress.

## 🚀 Features
- Receive drinking events from IoT bottles  
- Store events in per-bottle JSONL files  
- Get current water level, today's total intake, and graph data  
- Save & read bottle settings (goal, mode, reminder frequency)  
- Clear event history for a bottle  
- Generate mock data for all bottles  
- CORS enabled for use with React/MUI frontend  

## 📦 Project Structure (Backend)
| File | Description |
|------|-------------|
| `main.py` | Flask app, all API routes (events, settings, analytics) |
| `bottle.py` | Save/load events & bottle settings to JSON files |
| `app_utils.py` | Helpers for analytics: water level, drink graph, totals |
| `generate_mock_events.py` | Mock event generator for classroom testing |
| `config.py` | Global settings (paths, modes, bottle capacity) |
| `requirements.txt` | Dependencies list (Flask, CORS, requests) |

## ⚙️ Installation
```bash
git clone <your-repo-url>
cd backend
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

## ▶️ Run the Server
```bash
python main.py
```

The server runs on:
```
http://0.0.0.0:5003
```

(or your Render.com domain if deployed)

# 🛠 API Reference
All endpoints begin with:
```
/api/
```

# 🧴 Bottle Event Endpoints

### ➤ POST /api/bottle/<bottle_id>/events
Submit a drink event.

**Body:**
```json
{
  "amount_drank_ml": 200,
  "water_level_ml": 2700
}
```

**Example cURL:**
```bash
curl -X POST "https://iot-project-6i3k.onrender.com/api/bottle/8/events"   -H "Content-Type: application/json"   -d '{"amount_drank_ml":200,"water_level_ml":2700}'
```

# ⚙️ Bottle Settings

### ➤ GET /api/bottle/<bottle_id>/settings
Returns settings for the bottle.

### ➤ POST /api/bottle/<bottle_id>/settings
Update bottle configuration.

**Body:**
```json
{
  "mode": "hydration",
  "goal": 2500,
  "alerts_every": 60
}
```

# 📱 App (Frontend) Endpoints

### ➤ GET /api/app/<bottle_id>/water-level
Returns last known water level, capacity, and timestamp.

### ➤ GET /api/app/<bottle_id>/total-drank-today
Returns total amount drank today.

### ➤ GET /api/app/<bottle_id>/drink-amount-graph
Returns today's drinking events as graph points.

### ➤ POST /api/app/<bottle_id>/clear-event-data
Deletes all event history.

# 🧪 Mock Data

### ➤ POST /api/app/generate-mock-events
Regenerates mock JSONL event files for all bottles.

## 🗄 Data Storage
Events stored as JSONL:
```
data/bottle_events_<id>.jsonl
```

Settings stored in:
```
data/settings.json
```

## 🧪 Local Testing
```bash
curl https://iot-project-6i3k.onrender.com/api/app/8/water-level
curl -X POST https://iot-project-6i3k.onrender.com/api/app/8/clear-event-data
curl https://iot-project-6i3k.onrender.com/api/app/8/drink-amount-graph
```

## 📄 License
MIT
