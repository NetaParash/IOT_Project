# Smart Bottle – Frontend

This directory contains the **React-based frontend** for the **Smart Bottle IoT Project**, developed as part of an academic IoT system that monitors water consumption, tracks hydration progress, and provides real-time visualization for students or users.

The frontend communicates with a backend service using REST APIs and displays data such as water level, drinking history, daily goals, and customizable settings.

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Installation & Running](#installation--running)
4. [Project Structure](#project-structure)
5. [Configuration](#configuration)
6. [User Interface Documentation](#user-interface-documentation)
7. [System Modes & Statuses](#system-modes--statuses)
8. [Configurable Parameters](#configurable-parameters)
9. [Error Messages](#error-messages)
10. [Calibration Instructions](#calibration-instructions)
11. [Performance Tests](#performance-tests)
12. [Libraries & Versions](#libraries--versions)

---

##  Overview

The **Smart Bottle Frontend** is a single-page application built with **React**.  
It displays hydration data sent from IoT devices (“Smart Bottles”) used by individual students or users.

Main capabilities include:

- Real-time water-level monitoring
- Drinking-history graphs
- Daily hydration statistics
- Classroom view (monitor all bottles simultaneously)
- Configurable reminders and daily goals
- Settings page with predefined hydration modes

This folder contains *only the client side* of the system.

---

##  Features

- **Live dashboard** with automatic polling
- **SVG water bottle** with animated fill
- **Time-series graph** of drinking events
- **Classroom overview** of all bottles
- **Settings page** with multiple modes
- **Mock event generator** for testing
- Fully responsive layout using **Material UI**

---

##  Installation & Running

### 1. Install dependencies
```bash
npm install
```
### 2. Start development server
```bash
npm start
```

Application runs at:

http://localhost:3000/
### 3. Build for production
```bash
npm run build
```

## Project Structure
```
frontend/
│
├── App.js
├── AppContext.js
├── config.js
├── PagesConfig.js
├── theme.js
│
├── components/
│   ├── BottleDropdown.js
│   ├── DashboardCard.jsx
│   ├── DrinkingAmountGraph.jsx
│   ├── WaterLevel.jsx
│   ├── WaterLevelClassroomDashboard.js
│   ├── SettingsPage.js
│   ├── ClassroomPage.js
│   ├── DashboardPage.js
│   ├── WelcomePage.js
│   ├── SideDrawer.js
│   └── TopAppBar.js
│
├── dev.json
└── prod.json
```

# Configuration

Configuration files are located in:

- `config.js`
- `dev.json`
- `prod.json`

The active configuration is selected using the environment variable:

```
REACT_APP_ENVIRONMENT=dev | prod
```

## Example Fields

| Field | Description |
|-------|-------------|
| `API_BASE_URL` | URL of backend API |
| `FRONTEND_PORT` | Port used by the React app |
| `API_PORT` | Backend port |
| `API_KEY` | Optional key (unused in dev) |
| Endpoint names | Routes used to build API calls |

---

#  User Interface Documentation

## Welcome Page
- Project logo
- **Generate Mock Events** button
- **Delete Data** button (for bottle 1)

## Dashboard Page
- Bottle selector (dropdown)
- Animated water bottle SVG
- Daily goal display
- Drinking progress (%)
- Drinking history graph

## Classroom Page
Displays **all bottles** simultaneously, each card showing:
- Student name
- Progress percentage
- Mini water bottle visualization

## Settings Page
- Preset modes: **Hydration, Sport, Office, Night**
- Custom mode with manual configuration
- Saves changes via POST request

## Side Drawer Navigation
Pages:
- Welcome
- Dashboard
- Stats
- Settings

---

#  System Modes & Statuses

## Hydration Modes

| Mode | Goal (ml/day) | Interval | Description |
|------|---------------|----------|-------------|
| **Hydration** | 2500 | 60 min | Balanced intake |
| **Sport** | 3500 | 30 min | High activity |
| **Office** | 2000 | 90 min | Low reminders |
| **Night** | 500 | 0 | Minimal reminders |
| **Custom** | User-defined | User-defined | Manual mode |

---

# ️ Configurable Parameters

## Via UI
- `mode`
- `goal` (0–5000 ml)
- `alertsEvery` (0–180 minutes)
- Selected bottle ID

## Via Config File
- `API_BASE_URL`
- Backend endpoints
- Environment selection

## Via Code
- Polling interval
- Graph refresh speed
- Bottle capacity (`500 ml`)

---

# ️ Error Messages

## User-visible
- “Please choose a student”
- “No data so far”
- Errors when generating or deleting data

## Console (dev tools)
- `Polling failed`
- `Failed to load drink amount graph`
- `Settings save failed`

> These indicate backend issues but do **not** break the UI.

---

#  Calibration Instructions

The frontend requires **no calibration**, but the water fill animation assumes:

```
BOTTLE_CAPACITY_ML = 500
```

Located in:
```
WaterLevel.jsx
```

Update this value if using bottles with different capacity.