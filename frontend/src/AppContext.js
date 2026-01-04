import React, {
    createContext,
    useContext,
    useState,
    useEffect,
    useMemo,
} from "react";

import config from "./config";

const AppContext = createContext(null);

const SETTINGS_POLL_INTERVAL = 30_000; // 30 seconds

export const AppProvider = ({ children }) => {
    // ----- bottle state -----
    const [currentWaterLevel, setCurrentWaterLevel] = useState(0);
    const [dailyGoal, setDailyGoal] = useState(1500);
    const [totalDrankToday, setTotalDrankToday] = useState(0);

    // ----- settings -----
    const [mode, setMode] = useState(null);
    const [settingsLoading, setSettingsLoading] = useState(true);

    // ----- fetch settings -----
    const fetchSettings = async () => {
        try {
            const res = await fetch(
                `${config.API_BASE_URL}/api/bottle/settings`
            );
            const data = await res.json();

            if (typeof data.mode === "number") {
                setMode(data.mode);
            }
        } catch (err) {
            console.error("Failed to fetch settings:", err);
        } finally {
            setSettingsLoading(false);
        }
    };

    // ----- poll settings every 30s -----
    useEffect(() => {
        fetchSettings(); // initial fetch

        const interval = setInterval(fetchSettings, SETTINGS_POLL_INTERVAL);

        return () => clearInterval(interval);
    }, []);

    const value = useMemo(
        () => ({
            // bottle data
            currentWaterLevel,
            setCurrentWaterLevel,
            dailyGoal,
            setDailyGoal,
            totalDrankToday,
            setTotalDrankToday,

            // settings
            mode,
            setMode,
            settingsLoading,
        }),
        [
            currentWaterLevel,
            dailyGoal,
            totalDrankToday,
            mode,
            settingsLoading,
        ]
    );

    return (
        <AppContext.Provider value={value}>
            {children}
        </AppContext.Provider>
    );
};

export const useAppContext = () => {
    const context = useContext(AppContext);
    if (!context) {
        throw new Error("useAppContext must be used within AppProvider");
    }
    return context;
};
