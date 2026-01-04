import { Grid, Typography } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";
import DrinkingAmountGraph from "../components/DrinkingAmountGraph";
import { useAppContext } from "../AppContext";

export default function DashboardPage() {
    const { goal, drankToday } = useAppContext();

    return (
        <Grid container spacing={3}>
            {/* ===== Row 1: 3 cards ===== */}
                <Grid item size={3}>
                    <DashboardCard title="Water Level" subtitle="Current">
                        <WaterLevel />
                    </DashboardCard>
                </Grid>

                <Grid item size={3}>
                    <DashboardCard title="Daily Goal" subtitle="It's important to stay hydrated">
                        <Typography variant="h1">{goal} ml</Typography>
                    </DashboardCard>
                </Grid>

                <Grid item size={3}>
                    <DashboardCard title="My Progress" subtitle={`Today drank - ${drankToday} ml`}>
                        <Typography variant="h1">
                            {Math.round((drankToday * 100) / goal)} %
                        </Typography>
                    </DashboardCard>
                </Grid>

            {/* ===== Row 2: Graph ===== */}
            <Grid item size={9}>
                <DashboardCard title="Drinking Amount" subtitle="Today">
                    <DrinkingAmountGraph />
                </DashboardCard>
            </Grid>
        </Grid>
    );
}
