import {
    Box,
    Button,
    Divider,
    IconButton,
    Link,
    Slide,
    Typography
} from "@mui/material";
import React, { useState } from "react";
import GitHubIcon from "@mui/icons-material/GitHub";
import { useNavigate } from "react-router-dom";
import { useAppContext } from "../AppContext";
import config from "../config";

export default function WelcomePage() {
    const [loading, setLoading] = useState(false);
    const { setIsDrawerOpen } = useAppContext();
    const navigate = useNavigate();

    const handleGenerateMockEvents = async () => {
        try {
            setLoading(true);

            const res = await fetch(
                `${config.API_BASE_URL}/api/app/generate-mock-events`,
                { method: "POST" }
            );

            if (!res.ok) {
                throw new Error("Failed to generate mock events");
            }

            alert("Mock events generated successfully!");
        } catch (err) {
            console.error(err);
            alert("Something went wrong while generating mock events.");
        } finally {
            setLoading(false);
        }
    };

    return (
        <>
            <Slide
                direction="up"
                in={true}
                mountOnEnter
                unmountOnExit
                timeout={{ enter: 1000 }}
            >
                <Box
                    sx={{
                        display: "flex",
                        flexDirection: "column",
                        justifyContent: "center",
                        alignItems: "center",
                        padding: 2,
                        textAlign: "center",
                    }}
                >
                    <Typography variant="h1" component="h1" gutterBottom>
                        Welcome to Smart Bottle!
                    </Typography>

                    <Box
                        component="img"
                        alt="Or Zohar's Football Site Logo"
                        sx={{
                            maxHeight: { xs: 200, md: 200 },
                            maxWidth: { xs: 200, md: 250 },
                            padding: 2,
                        }}
                        src="../../logo.png"
                    />

                    {/* 🔥 Generate Mock Events Button */}
                    <Button
                        variant="contained"
                        color="primary"
                        onClick={handleGenerateMockEvents}
                        disabled={loading}
                        sx={{
                            mt: 3,
                            fontSize: 18,
                            paddingX: 3,
                            paddingY: 1.5,
                            borderRadius: 2
                        }}
                    >
                        {loading ? "Working..." : "Generate Mock Events"}
                    </Button>
                </Box>
            </Slide>
        </>
    );
}
