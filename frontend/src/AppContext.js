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

const NAMES_BY_ID = {
    1: null,
    2: "Neta",
    3: "Or",
    4: "Tom",
    5: "Lama",
    6: "Itay",
    7: "Avigail",
    8: "Wesam",
    9: "Ilay",
    10: "Ido",
    11: "Noa",
    12: "Daniel",
    13: "Alon",
    14: "Eden",
    15: "Tomer",
    16: "Noga",
    17: "Omer",
    18: "Rotem",
    19: "Gal",
    20: "Yuval"
};


export const AppProvider = ({ children }) => {
    // ----- Global state -----
    const [waterLevel, setWaterLevel] = useState(0);
    const [drankToday, setDrankToday] = useState(0);

    // Bottle selection
    const [selectedBottleId, setSelectedBottleId] = useState(null);

    // Bottle list (id + name)
    const [bottles, setBottles] = useState(() => {
        const saved = localStorage.getItem("bottles");
        if (saved) return JSON.parse(saved);

        return Object.entries(NAMES_BY_ID).map(([id, name]) => ({
            id: Number(id),
            name: name ?? `Bottle ${id}`
        }));
    });
    useEffect(() => {
        localStorage.setItem("bottles", JSON.stringify(bottles));
    }, [bottles]);

    const updateBottleName = (id, newName) => {
        setBottles(prev =>
            prev.map(b => b.id === id ? { ...b, name: newName } : b)
        );
    };

    const addBottle = (name) => {
        setBottles(prev => {
            const maxId = prev.length === 0 ? 0 : Math.max(...prev.map(b => b.id));
            const newBottle = {
                id: maxId + 1,
                name: name ?? null
            };
            return [...prev, newBottle];
        });
    };



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
            updateBottleName,
            addBottle,

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
            bottles,
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
