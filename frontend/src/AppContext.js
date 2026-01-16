import React, {
    createContext,
    useContext,
    useEffect,
    useMemo,
    useState,
} from "react";

import config from "./config";

const AppContext = createContext(null);
const POLL_INTERVAL_MS = 30_000;

const NAMES = [
    "Liat", "Noa", "Yonatan", "Tal", "Gili",
    "Roni", "Shira", "Amit", "Inbar", "Maayan",
    "Yael", "Daniel", "Alon", "Eden", "Tomer",
    "Noga", "Omer", "Rotem", "Gal", "Yuval"
];

export const AppProvider = ({ children }) => {
    // ----- Global state -----
    const [waterLevel, setWaterLevel] = useState(0);
    const [drankToday, setDrankToday] = useState(0);

    // Bottle selection
    const [selectedBottleId, setSelectedBottleId] = useState(null);

    // Bottle list (id + name)
    const [bottles] = useState(
        NAMES.map((name, index) => ({
            id: index + 1,
            name
        }))
    );

    const [mode, setMode] = useState(null);
    const [goal, setGoal] = useState(0);
    const [alertsEvery, setAlertsEvery] = useState(0);

    const [isDrawerOpen, setIsDrawerOpen] = useState(true);
    const toggleDrawer = () => setIsDrawerOpen(!isDrawerOpen);

    // ----- Poll backend -----
    const pollBackend = async () => {
        if (!selectedBottleId) return;   // 🔥 only poll for selected bottle

        try {
            // settings
            const settingsRes = await fetch(
                `${config.API_BASE_URL}/api/app/${selectedBottleId}/settings`
            );
            const settings = await settingsRes.json();

            setMode(settings.mode);
            setGoal(settings.goal);
            setAlertsEvery(settings.alerts_every);

            // water level
            const waterRes = await fetch(
                `${config.API_BASE_URL}/api/app/${selectedBottleId}/water-level`
            );
            const water = await waterRes.json();
            setWaterLevel(water.water_level_ml);

            // total drank today
            const drankRes = await fetch(
                `${config.API_BASE_URL}/api/app/${selectedBottleId}/total-drank-today`
            );
            const drank = await drankRes.json();
            setDrankToday(drank.total_drank_today_ml);
        } catch (err) {
            console.error("Polling failed:", err);
        }
    };

    useEffect(() => {
        pollBackend(); // immediate fetch on bottle select
        const interval = setInterval(pollBackend, POLL_INTERVAL_MS);
        return () => clearInterval(interval);
    }, [selectedBottleId]); // 🔥 run again when bottle changes

    // ----- Memoize exposed context -----
    const value = useMemo(
        () => ({
            waterLevel,
            drankToday,

            mode,
            goal,
            alertsEvery,

            setMode,
            setGoal,
            setAlertsEvery,

            selectedBottleId,
            setSelectedBottleId,
            bottles,

            isDrawerOpen,
            toggleDrawer,
        }),
        [
            waterLevel,
            drankToday,
            mode,
            goal,
            alertsEvery,
            selectedBottleId,
            isDrawerOpen
        ]
    );

    return (
        <AppContext.Provider value={value}>
            {children}
        </AppContext.Provider>
    );
};

export const useAppContext = () => {
    const ctx = useContext(AppContext);
    if (!ctx) {
        throw new Error("useAppContext must be used within AppProvider");
    }
    return ctx;
};
