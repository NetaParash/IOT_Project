import React, { useEffect, useState } from "react";
import { LineChart } from "@mui/x-charts/LineChart";
import { Box } from "@mui/material";
import Typography from "@mui/material/Typography";
import config from "../config";
import { useAppContext } from "../AppContext";
import { ChartsReferenceLine } from "@mui/x-charts/ChartsReferenceLine";

const POLL_INTERVAL_MS = 1_000;

export default function DrinkingAmountGraph() {
    const [points, setPoints] = useState([]);
    const { selectedBottleId, goal } = useAppContext();

    const fetchTodayHistory = async () => {
        if (!selectedBottleId) return;

        try {
            const res = await fetch(
                `${config.API_BASE_URL}/api/app/${selectedBottleId}/drink-amount-graph`
            );

            const json = await res.json();


            const parsed = json.map((p) => ({
                x: new Date(p.x).getTime(),
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
        if (!selectedBottleId) return;

        // 🔥 reset graph when bottle changes
        setPoints([]);

        fetchTodayHistory();

        const interval = setInterval(fetchTodayHistory, POLL_INTERVAL_MS);
        return () => clearInterval(interval);

    }, [selectedBottleId]); // 🔥 bottle-aware

    if (!selectedBottleId) {
        return (
            <Typography
                variant="h6"
                sx={{ textAlign: "center", mt: 2, color: "text.secondary" }}
            >
                Please choose a student
            </Typography>
        );
    }

    if (points.length === 0) {
        return (
            <Typography
                variant="h2"
                sx={{ textAlign: "center", mt: 2, color: "text.secondary" }}
            >
                No data so far
            </Typography>
        );
    }

    return (
        <Box height={600}>
            <LineChart
                dataset={points}
                grid={{ horizontal: true, vertical: true }}

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
                        valueFormatter: (ts) =>
                            new Date(ts).toLocaleTimeString([], {
                                hour: "2-digit",
                                minute: "2-digit",
                            }),
                    },
                ]}

                yAxis={[
                    {
                        label: "ml",
                        min: 0,
                        max: 1.25*goal,
                    },
                ]}

                margin={{ left: 60, right: 20, top: 20, bottom: 40 }}
            >
                <ChartsReferenceLine
                    y={goal}
                    label={`Daily Goal: ${goal} ml`}
                    lineStyle={{ stroke: "#4CAF50" }}
                    labelStyle={{
                        fontSize: 20,
                        fontWeight: 600,
                    }}
                />
            </LineChart>
        </Box>
    );
}
