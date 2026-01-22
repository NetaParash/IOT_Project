import {
    Grid,
    Typography,
    Slider,
    ToggleButton,
    ToggleButtonGroup,
    Button,
    Box,
    TextField,
} from "@mui/material";
import { useAppContext } from "../AppContext";
import DashboardCard from "../components/DashboardCard";
import config from "../config";
import BottleDropdown from "./BottleDropdown";
import { useState } from "react";
import theme from "../theme";

const MODES = {
    hydration: { label: "Hydration", dailyGoalMl: 2500, alertEveryMinutes: 60 },
    sport: { label: "Sport", dailyGoalMl: 3500, alertEveryMinutes: 30 },
    office: { label: "Office", dailyGoalMl: 2000, alertEveryMinutes: 90 },
    night: { label: "Night", dailyGoalMl: 500, alertEveryMinutes: 0 },
    custom: { label: "Custom" },
};

export default function SettingsPage() {
    const {
        mode,
        goal,
        alertsEvery,
        setMode,
        setGoal,
        setAlertsEvery,
        selectedBottleId,
        updateBottleName,
    } = useAppContext();

    const isCustom = mode === "custom";

    const [newBottleName, setNewBottleName] = useState("");

    const updateSettings = async () => {
        await fetch(`${config.API_BASE_URL}/api/app/${selectedBottleId}/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                mode,
                goal,
                alerts_every: alertsEvery,
            }),
        });
    };

    const onModeChange = (_, newMode) => {
        if (!newMode) return;

        setMode(newMode);

        if (newMode !== "custom") {
            const preset = MODES[newMode];
            setGoal(preset.dailyGoalMl);
            setAlertsEvery(preset.alertEveryMinutes);
        }
    };

    const handleSetClick = async () => {
        await updateSettings();

        if (newBottleName.trim() !== "") {
            updateBottleName(selectedBottleId, newBottleName.trim());
            setNewBottleName("");
        }
    };

    return (
        <>
            <BottleDropdown />
            <Grid container spacing={3}>


                {/* ===== MODE (full row) ===== */}
                <Grid item size={12}>
                    <DashboardCard title="Mode" subtitle="Bottle behaviour">
                        <ToggleButtonGroup
                            exclusive
                            value={mode}
                            onChange={onModeChange}
                            size="large"
                            sx={{ flexWrap: "wrap" }}
                        >
                            {Object.entries(MODES).map(([key, cfg]) => (
                                <ToggleButton key={key} value={key}>
                                    {cfg.label}
                                </ToggleButton>
                            ))}
                        </ToggleButtonGroup>
                    </DashboardCard>
                </Grid>

                {/* ===== DAILY GOAL (full row) ===== */}
                <Grid item size={12}>
                    <DashboardCard title="Daily Goal" subtitle="ml per day">
                        <Typography variant="h1" mb={2}>
                            {goal} ml
                        </Typography>
                        <Slider
                            value={goal}
                            min={0}
                            max={5000}
                            step={100}
                            disabled={!isCustom}
                            onChange={(_, v) => setGoal(v)}
                            sx={{ height: 12 }}
                        />
                    </DashboardCard>
                </Grid>

                {/* ===== ALERTS (full row) ===== */}
                <Grid item size={12}>
                    <DashboardCard title="Alerts" subtitle="Reminder interval">
                        <Typography variant="h1" mb={2}>
                            {alertsEvery} min
                        </Typography>
                        <Slider
                            value={alertsEvery}
                            min={0}
                            max={180}
                            step={1}
                            disabled={!isCustom}
                            onChange={(_, v) => setAlertsEvery(v)}
                            sx={{ height: 12 }}
                        />
                    </DashboardCard>
                </Grid>

                {/* ===== ROW 4: Bottle Name (left) + Set Button (right) ===== */}

                    {/* Left half: Bottle Name */}
                    <Grid item size={6}>
                        <DashboardCard title="Bottle Name" subtitle="">
                            <TextField
                                label="Bottle name"
                                variant="outlined"
                                value={newBottleName}
                                onChange={(e) => setNewBottleName(e.target.value)}
                                sx={{
                                    "& .MuiInputBase-input": {
                                        ...theme.typography.h1,   // 👈 APPLY h4 style here
                                    }
                                }}


                            />
                        </DashboardCard>
                    </Grid>

                    {/* Right half: Set Button */}
                    <Grid size={6}>
                        <DashboardCard title="Apply Settings">
                            <Box mt={2}>
                                <Button
                                    variant="contained"
                                    size="large"
                                    sx={{ height: 64, fontSize: "1.4rem" }}
                                    onClick={handleSetClick}
                                >
                                    Set
                                </Button>
                            </Box>
                        </DashboardCard>
                    </Grid>


            </Grid>
        </>
    );
}
