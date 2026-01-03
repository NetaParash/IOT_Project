import { Grid } from "@mui/material";
import DashboardCard from "../components/DashboardCard";
import WaterLevel from "../components/WaterLevel";

export default function DashboardPage() {
    return (
        <Grid container spacing={3} p={3}>
            <Grid item xs={12} md={4}>
                <DashboardCard title="Water Level" subtitle="Current">
                    <WaterLevel />
                </DashboardCard>
            </Grid>
        </Grid>
    );
}
