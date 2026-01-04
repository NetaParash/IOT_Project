import React, { useEffect, useState } from "react";
import { LineChart } from "@mui/x-charts/LineChart";
import { CircularProgress, Box } from "@mui/material";

export default function DrinkingAmountGraph() {
    const [points, setPoints] = useState(null);

    useEffect(() => {
        async function fetchTodayHistory() {
            try {
                const res = await fetch(
                    "http://localhost:5003/api/app/drink-amount-graph"
                );
                const json = await res.json();

                const parsed = json.map(p => ({
                    x: new Date(p.x),
                    y: p.y,
                }));
                setPoints(parsed);
            } catch (err) {
                console.error("Failed to load drink amount graph", err);
            }
        }

        fetchTodayHistory();
    }, []);

    if (!points) {
        return <CircularProgress size={24} />;
    }

    return (
        <Box height={600}>
            <LineChart
                dataset={points}                 // ✅ REQUIRED
                series={[
                    {
                        dataKey: "y",           // ✅ cumulative amount
                        label: "Amount drank (ml)",
                        color: "#3A7BD5",
                    },
                ]}
                xAxis={[
                    {
                        dataKey: "x",           // ✅ ISO timestamp
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
