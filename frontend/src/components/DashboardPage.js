import { Grid } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";
import DrinkingAmountGraph from "../components/DrinkingAmountGraph";

export default function DashboardPage() {
    return (
        <Grid container spacing={3} p={3}>
            {/* Bottle */}
            <Grid item xs={12} md={4}>
                <DashboardCard
                    title="Water Level"
                    subtitle="Current"
                >
                    <WaterLevel />
                </DashboardCard>
            </Grid>

            {/* Drink amount graph */}
            <Grid item xs={12} md={8}>
                <DashboardCard
                    title="Drinking Amount"
                    subtitle="Today"
                >
                    <DrinkingAmountGraph />
                </DashboardCard>
            </Grid>
        </Grid>
    );
}
