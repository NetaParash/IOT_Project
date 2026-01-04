import React, { useEffect, useState } from "react";
import { LineChart } from "@mui/x-charts/LineChart";
import { CircularProgress, Box } from "@mui/material";
import config from "../config";
import { useAppContext } from "../AppContext";
import { ChartsReferenceLine } from "@mui/x-charts/ChartsReferenceLine";


const POLL_INTERVAL_MS = 30_000;

export default function DrinkingAmountGraph() {
    const [points, setPoints] = useState([]);
    const { goal } = useAppContext();

    const fetchTodayHistory = async () => {
        try {
            const res = await fetch(
                `${config.API_BASE_URL}/api/app/drink-amount-graph`
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
                        max: goal,
                    },
                ]}
                margin={{ left: 60, right: 20, top: 20, bottom: 40 }}
            >  <ChartsReferenceLine
                y={goal}
                label={`Daily Goal: ${goal} ml`}
                lineStyle={{ stroke: "#4CAF50"}}
                labelStyle={{
                    fontSize: 20,
                    fontWeight: 600,

                }}
            />
            </LineChart>
        </Box>
    );
}
