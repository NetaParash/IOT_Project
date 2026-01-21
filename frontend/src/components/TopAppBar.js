import React from "react";
import { AppBar, Toolbar, Typography, IconButton, Box } from "@mui/material";
import DehazeOutlinedIcon from '@mui/icons-material/DehazeOutlined';
import CloseIcon from '@mui/icons-material/Close';
import { useAppContext } from "../AppContext";
import { useLocation } from "react-router-dom";
import { appPages } from "../PagesConfig";


export default function TopAppBar() {
    const { toggleDrawer, isDrawerOpen, selectedBottleId, bottles } = useAppContext();
    const location = useLocation();

    const appBarStyles = {
        height: 64,
        zIndex: (theme) => theme.zIndex.drawer + 1,
        backgroundColor: 'rgb(24,201,241)',
    };

    const logoStyles = {
        height: 50,
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
    };

    // Page title resolution
    const currentPage = appPages.find(
        (page) => page.path.toLowerCase() === location.pathname.toLowerCase()
    );

    let currentTitle = currentPage ? currentPage.name : "Page Not Found";

    if (currentTitle === "Calendar") {
        currentTitle = "Calendar - rotate for better view!";
    }

    // ✔ Convert bottleId → name (IDs start at 1)
    const selectedBottleName = selectedBottleId
        ? bottles.find(b => b.id === selectedBottleId)?.name
        : null;

    return (
        <AppBar position="fixed" sx={appBarStyles}>
            <Toolbar>
                {/* Drawer toggle */}
                <IconButton
                    edge="start"
                    color="inherit"
                    aria-label={isDrawerOpen ? "close menu" : "open menu"}
                    onClick={toggleDrawer}
                >
                    {isDrawerOpen ? <CloseIcon /> : <DehazeOutlinedIcon />}
                </IconButton>

                {/* Title */}
                <Typography variant="h1" sx={{ flexGrow: 1 }}>
                    Smart Bottle – {currentTitle}
                    {selectedBottleName && ` – ${selectedBottleName}`}
                </Typography>

                {/* Logo */}
                <Box
                    component="img"
                    src="../../logo.png"
                    sx={logoStyles}
                />
            </Toolbar>
        </AppBar>
    );
}
