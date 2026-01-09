import * as React from "react";
import {
    Card,
    CardContent,
    Typography,
    Box,
    LinearProgress,
} from "@mui/material";
import LocalDrinkIcon from "@mui/icons-material/LocalDrink";

export default function DailyGoalBox({
                                         goalMl = 2000,
                                         drankMl = 0,
                                     }) {
    const progress = Math.min((drankMl / goalMl) * 100, 100);
    const remaining = Math.max(goalMl - drankMl, 0);

    return (
        <Card
            sx={{
                borderRadius: 3,
                boxShadow: 3,
                minWidth: 280,
            }}
        >
            <CardContent>
                {/* Header */}
                <Box sx={{ display: "flex", alignItems: "center", mb: 2 }}>
                    <Box
                        sx={{
                            width: 48,
                            height: 48,
                            borderRadius: "50%",
                            bgcolor: "primary.main",
                            color: "white",
                            display: "flex",
                            alignItems: "center",
                            justifyContent: "center",
                            mr: 2,
                        }}
                    >
                        <LocalDrinkIcon />
                    </Box>

                    <Typography variant="h6" fontWeight="bold">
                        Daily Water Goal
                    </Typography>
                </Box>

                {/* Main value */}
                <Typography variant="h4" fontWeight="bold">
                    {drankMl} ml
                </Typography>

                <Typography variant="body2" color="text.secondary" gutterBottom>
                    of {goalMl} ml
                </Typography>

                {/* Progress */}
                <LinearProgress
                    variant="determinate"
                    value={progress}
                    sx={{
                        height: 8,
                        borderRadius: 5,
                        my: 2,
                    }}
                />

                {/* Footer */}
                <Typography variant="caption" color="text.secondary">
                    {remaining > 0
                        ? `${remaining} ml remaining`
                        : "Goal reached 🎉"}
                </Typography>
            </CardContent>
        </Card>
    );
}
