import { Grid, Typography } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";
import DrinkingAmountGraph from "../components/DrinkingAmountGraph";
import { useAppContext } from "../AppContext";
import BottleDropdown from "../components/BottleDropdown";

export default function DashboardPage() {
    const { goal, drankToday } = useAppContext();

    return (
        <>
            <BottleDropdown />
            {/* ===== Row 1 ===== */}
            <Grid container spacing={3}>
                <Grid item xs={12} md={4}>
                    <DashboardCard title="Water Level" subtitle="Current">
                        <WaterLevel />
                    </DashboardCard>
                </Grid>

                <Grid item xs={12} md={4}>
                    <DashboardCard
                        title="Daily Goal"
                        subtitle="It's important to stay hydrated"
                    >
                        <Typography variant="h1">{goal} ml</Typography>
                    </DashboardCard>
                </Grid>

                <Grid item xs={12} md={4}>
                    <DashboardCard
                        title="My Progress"
                        subtitle={`Today drank - ${drankToday} ml`}
                    >
                        <Typography variant="h1">
                            {Math.round((drankToday * 100) / goal)} %
                        </Typography>
                    </DashboardCard>
                </Grid>
            </Grid>

            {/* ===== Row 2 (FULL WIDTH) ===== */}
            <Grid container spacing={3} sx={{ mt: 2 }}>
                <Grid item xs={12} sx={{ width: "80%" }} >
                    <DashboardCard title="Drinking Amount" subtitle="Today">
                        <DrinkingAmountGraph />
                    </DashboardCard>
                </Grid>
            </Grid>
        </>
    );
}