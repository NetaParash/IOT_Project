import React, { useEffect, useRef, useState } from "react";
import BottleLiquidGauge from "./BottleLiquidGauge";

const POLL_INTERVAL_MS = 30_000;

export default function BottleGaugeContainer() {
    const [percent, setPercent] = useState(0);

    // Prevent unnecessary re-renders
    const lastPercentRef = useRef(0);

    async function fetchWaterLevel() {
        try {
            const res = await fetch(
                "https://iot-project-6i3k.onrender.com/api/app/water-level"
            );
            const data = await res.json();

            const nextPercent = Math.floor(
                (data.water_level_ml / data.capacity_ml) * 100
            );

            // 🔥 only update state if value actually changed
            if (nextPercent !== lastPercentRef.current) {
                lastPercentRef.current = nextPercent;
                setPercent(nextPercent);
            }
        } catch (err) {
            console.error("Water level fetch failed", err);
        }
    }

    useEffect(() => {
        fetchWaterLevel(); // initial load

        const id = setInterval(fetchWaterLevel, POLL_INTERVAL_MS);
        return () => clearInterval(id);
    }, []);

    return <BottleLiquidGauge percent={percent} />;
}
