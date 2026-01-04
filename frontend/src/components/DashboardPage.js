import { Grid } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";
import DrinkingAmountGraph from "../components/DrinkingAmountGraph";

export default function DashboardPage() {
    return (
        <Grid container spacing={3} p={3}>
            {/* Bottle */}
            <Grid item size={3}>
                <DashboardCard
                    title="Water Level"
                    subtitle="Current"
                >
                    <WaterLevel />
                </DashboardCard>
            </Grid>
            <Grid item size={3}>
                <DashboardCard
                    title="Daily Goal"
                    subtitle="2000 ml"
                >
                </DashboardCard>
            </Grid>

            {/* Drink amount graph */}
            <Grid item size={10}>
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
