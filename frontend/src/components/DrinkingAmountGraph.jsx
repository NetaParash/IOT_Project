import React, { useEffect, useState } from "react";
import { LineChart } from "@mui/x-charts/LineChart";
import { CircularProgress, Box } from "@mui/material";

const POLL_INTERVAL_MS = 30_000;

export default function DrinkingAmountGraph() {
    const [points, setPoints] = useState([]);

    const fetchTodayHistory = async () => {
        try {
            const res = await fetch(
                "http://localhost:5003/api/app/drink-amount-graph"
            );
            const json = await res.json();

            const parsed = json.map((p) => ({
                x: new Date(p.x).getTime(), // use number, faster compare
                y: p.y,
            }));

            setPoints((prev) => {
                if (prev.length === 0) return parsed;

                const lastTs = prev[prev.length - 1].x;
                const newPoints = parsed.filter((p) => p.x > lastTs);

                return newPoints.length > 0
                    ? [...prev, ...newPoints]
                    : prev;
            });
        } catch (err) {
            console.error("Failed to load drink amount graph", err);
        }
    };

    useEffect(() => {
        fetchTodayHistory();

        const interval = setInterval(fetchTodayHistory, POLL_INTERVAL_MS);
        return () => clearInterval(interval);
    }, []);

    if (points.length === 0) {
        return <CircularProgress size={24} />;
    }

    return (
        <Box height={600}>
            <LineChart
                dataset={points}
                series={[
                    {
                        dataKey: "y",
                        label: "Amount drank (ml)",
                        color: "#3A7BD5",
                    },
                ]}
                xAxis={[
                    {
                        dataKey: "x",
                        scaleType: "time",
                        label: "Time (today)",
                    },
                ]}
                yAxis={[
                    {
                        label: "ml",
                        min: 0,
                        max: 500,
                    },
                ]}
                margin={{ left: 60, right: 20, top: 20, bottom: 40 }}
            />
        </Box>
    );
}
