import {Box, Divider, IconButton, Link, Slide, Typography} from "@mui/material";
import React, {useState} from "react";
import GitHubIcon from "@mui/icons-material/GitHub";
import {useNavigate} from "react-router-dom";
import {useAppContext} from "../AppContext"; // Assuming you're using React Router

export default function WelcomePage() {
    const [showDropdown, setShowDropdown] = useState(true);
    const {setIsDrawerOpen} = useAppContext();
    const navigate = useNavigate();


    return (<>
            <Slide direction="up" in={true} mountOnEnter unmountOnExit timeout={{
                enter: 1000,
            }}>
                <Box
                    sx={{
                        display: 'flex',
                        flexDirection: 'column',
                        justifyContent: 'center',
                        alignItems: 'center',
                        padding: 2,
                        position: 'center',
                        textAlign: 'center',
                    }}
                >
                    <Typography variant="h1" component="h1" gutterBottom>
                        Welcome to Smart Bottle!
                    </Typography>
                    <Box
                        component="img"
                        alt="Or Zohar's Football Site Logo"
                        sx={{
                            maxHeight: {xs: 200, md: 200},
                            maxWidth: {xs: 200, md: 250},
                            padding: 2,
                        }}
                        src="../../logo.png"
                    />

                </Box>
            </Slide>

        </>
    );
}
