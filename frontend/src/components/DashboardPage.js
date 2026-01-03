import React from "react";
import { Grid } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import BottleLiquidGauge from "../components/BottleLiquidGauge";

export async function fetchBottleState() {
    const res = await fetch("https://iot-project-6i3k.onrender.com/api/bottle/state");

    if (!res.ok) {
        throw new Error("Failed to fetch bottle state");
    }

    return res.json();
}

export default function DashboardPage() {
    const waterLevel = 500;
    const percent = Math.floor((waterLevel / 1000) * 100);

    return (
        <Grid container spacing={3} p={3}>
            <Grid item xs={12} md={4}>
                <DashboardCard
                    title="Bottle State"
                    subtitle="Now"
                >
                    <BottleLiquidGauge percent={percent} />
                </DashboardCard>
            </Grid>
        </Grid>
    );
}
