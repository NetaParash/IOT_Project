import React, { useEffect, useState } from "react";
import { LineChart } from "@mui/x-charts/LineChart";
import { CircularProgress, Box } from "@mui/material";

export default function DrinkingAmountGraph() {
    const [points, setPoints] = useState(null);

    useEffect(() => {
        async function fetchTodayHistory() {
            try {
                const res = await fetch(
                    "https://iot-project-6i3k.onrender.com/api/app/drink-amount-graph"
                );
                const json = await res.json();

                const data = json
                    .sort((a, b) => a.ts - b.ts)
                    .map(e => ({
                        x: new Date(e.ts * 1000),
                        y: e.amount_ml,
                    }));


                setPoints(data);
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
        <Box height={220}>
            <LineChart
                series={[
                    {
                        data: points,
                        label: "Amount drank (ml)",
                        color: "#3A7BD5",
                    },
                ]}
                xAxis={[
                    {
                        scaleType: "time",
                        dataKey: "x",
                        label: "Time (today)",
                    },
                ]}
                yAxis={[
                    {
                        label: "ml",
                    },
                ]}
                margin={{ left: 60, right: 20, top: 20, bottom: 40 }}
            />
        </Box>
    );
}
