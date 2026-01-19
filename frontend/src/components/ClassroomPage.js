import React, { useEffect, useState } from "react";
import { Grid, CircularProgress, Box } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevelClassroomDashboard from "../components/WaterLevelClassroomDashboard";
import config from "../config";
import { useAppContext } from "../AppContext";


export default function ClassroomPage() {
    const [percentages, setPercentages] = useState({});
    const [loading, setLoading] = useState(true);
    const { bottles } = useAppContext();

    useEffect(() => {
        let cancelled = false;

        const fetchBottleData = async (bottleId) => {
            try {
                const settingsRes = await fetch(
                    `${config.API_BASE_URL}/api/app/${bottleId}/settings`
                );
                const settings = await settingsRes.json();
                const goal = settings.goal;

                const drankRes = await fetch(
                    `${config.API_BASE_URL}/api/app/${bottleId}/total-drank-today`
                );
                const drankJson = await drankRes.json();
                const drank = drankJson.total_drank_today_ml ?? 0;

                const percent =
                    goal > 0
                        ? Math.min(100, Math.floor((drank / goal) * 100))
                        : 0;

                return { bottleId, percent };
            } catch (err) {
                console.error(`Bottle ${bottleId} failed`, err);
                return { bottleId, percent: 0 };
            }
        };

        const loadAll = async () => {
            setLoading(true);

            const results = await Promise.all(
                bottles.map(b => fetchBottleData(b.id))
            );

            if (cancelled) return;

            const map = {};
            results.forEach(({ bottleId, percent }) => {
                map[bottleId] = percent;
            });

            setPercentages(map);
            setLoading(false);
        };

        loadAll();

        return () => {
            cancelled = true;
        };
    }, []);
    if (loading) {
        return (
            <Box sx={{ display: "flex", justifyContent: "center", mt: 4 }}>
                <CircularProgress />
            </Box>
        );
    }

    return (
        <Grid container spacing={3}>
            {bottles.map((bottle) => (
                <Grid
                    item
                    xs={10}
                    sm={6}
                    md={5}   // ← 5 per row
                    key={bottle.id}
                >
                    <DashboardCard
                        title={bottle.name}
                        subtitle="Daily goal"
                        sx={{ height: "50%" }}
                    >
                        <WaterLevelClassroomDashboard percent={percentages[bottle.id] ?? 0} />
                    </DashboardCard>
                </Grid>
            ))}
        </Grid>
    );
}
