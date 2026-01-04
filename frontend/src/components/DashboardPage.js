import { Grid, Typography } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";
import DrinkingAmountGraph from "../components/DrinkingAmountGraph";
import { useAppContext } from "../AppContext";

export default function DashboardPage() {
    const { goal, drankToday } = useAppContext();

    return (
        <Grid container spacing={3} p={3}>
            {/* ===== Row 1: 3 cards ===== */}
            <Grid container item xs={12} spacing={3}>
                <Grid item xs={12} md={4}>
                    <DashboardCard title="Water Level" subtitle="Current">
                        <WaterLevel />
                    </DashboardCard>
                </Grid>

                <Grid item xs={12} md={4}>
                    <DashboardCard title="Daily Goal" subtitle={`${goal} ml`}>
                        <Typography variant="h4">{goal} ml</Typography>
                    </DashboardCard>
                </Grid>

                <Grid item xs={12} md={4}>
                    <DashboardCard title="My Progress" subtitle="Today">
                        <Typography variant="h4">{drankToday} ml</Typography>
                    </DashboardCard>
                </Grid>
            </Grid>

            {/* ===== Row 2: Graph ===== */}
            <Grid item xs={12}>
                <DashboardCard title="Drinking Amount" subtitle="Today">
                    <DrinkingAmountGraph />
                </DashboardCard>
            </Grid>
        </Grid>
    );
}
