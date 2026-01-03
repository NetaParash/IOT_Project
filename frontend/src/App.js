import React, { useEffect } from "react";
import TopAppBar from "./components/TopAppBar";
import { HashRouter as Router, useLocation } from "react-router-dom";
import MainContent from "./components/MainContent";
import "./App.css";
import { AppProvider } from "./AppContext";
import { CssBaseline } from "@mui/material";
import { ThemeProvider } from "@mui/material/styles";
import theme from "./theme";

function ScrollToTop() {
    const { pathname } = useLocation();

    useEffect(() => {
        window.scrollTo(0, 0);
        document.body.style.zoom = "70%";
    }, [pathname]);

    return null;
}

function App() {
    return (

            <AppProvider>
                <Router>
                    <ScrollToTop />
                    <TopAppBar />
                    <MainContent />
                </Router>
            </AppProvider>
    );
}

export default App;
