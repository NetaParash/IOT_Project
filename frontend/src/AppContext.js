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

export const AppProvider = ({ children }) => {
    // ---- authoritative state (5 only) ----
    const [waterLevel, setWaterLevel] = useState(0);
    const [drankToday, setDrankToday] = useState(0);

    const [mode, setMode] = useState(null);
    const [goal, setGoal] = useState(0);
    const [alertsEvery, setAlertsEvery] = useState(0);
    const [isDrawerOpen, setIsDrawerOpen] = useState(true);
    const toggleDrawer = () => {
        setIsDrawerOpen(!isDrawerOpen);
    };

    const pollBackend = async () => {
        try {
            // settings
            const settingsRes = await fetch(
                `${config.API_BASE_URL}/api/app/settings`
            );
            const settings = await settingsRes.json();

            setMode(settings.mode);
            setGoal(settings.goal);
            setAlertsEvery(settings.alerts_every);

            // water level
            const waterRes = await fetch(
                `${config.API_BASE_URL}/api/app/water-level`
            );
            const water = await waterRes.json();

            setWaterLevel(water.water_level_ml);

            // total drank today
            const drankRes = await fetch(
                `${config.API_BASE_URL}/api/app/total-drank-today`
            );
            const drank = await drankRes.json();

            setDrankToday(drank.total_drank_today_ml);
        } catch (err) {
            console.error("Polling failed:", err);
        }
    };

    useEffect(() => {
        pollBackend(); // initial
        const interval = setInterval(pollBackend, POLL_INTERVAL_MS);
        return () => clearInterval(interval);
    }, []);

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

            isDrawerOpen,
            toggleDrawer,
        }),
        [waterLevel, drankToday, mode, goal, alertsEvery, isDrawerOpen]
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
