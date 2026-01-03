import React from "react";
import { Card, CardContent, Box, Typography } from "@mui/material";

export default function DashboardCard({
                                          title,
                                          subtitle,
                                          action,
                                          children,
                                      }) {
    return (
        <Card
            elevation={0}
            sx={{
                borderRadius: 3,
                border: "1px solid",
                borderColor: "divider",
                height: "100%",
            }}
        >
            <CardContent>
                {/* Header */}
                {(title || action) && (
                    <Box
                        display="flex"
                        justifyContent="space-between"
                        alignItems="center"
                        mb={1}
                    >
                        <Box>
                            {title && (
                                <Typography variant="h2" color="text.secondary">
                                    {title}
                                </Typography>
                            )}
                            {subtitle && (
                                <Typography variant="h5" color="text.secondary">
                                    {subtitle}
                                </Typography>
                            )}
                        </Box>

                        {action}
                    </Box>
                )}

                {/* Content */}
                <Box mt={2}>{children}</Box>
            </CardContent>
        </Card>
    );
}
