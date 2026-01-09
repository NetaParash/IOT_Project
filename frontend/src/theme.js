// src/theme.js
import { createTheme } from "@mui/material/styles";

const theme = createTheme({
    palette: {
        mode: "light",
        primary: {
            main: "#3A7BD5",
        },
        background: {
            default: "#f5f7fa",
            paper: "#ffffff",
        },
    },
    shape: {
        borderRadius: 12,
    },
    typography: {
        fontSize: 12,
        h1: {
            fontSize: "2.2rem",
            "@media (max-width:600px)": { fontSize: "1.8rem" },
        },
        h2: {
            fontSize: "1.8rem",
            "@media (max-width:600px)": { fontSize: "1.6rem" },
        },
        h3: {
            fontSize: "1.5rem",
            "@media (max-width:600px)": { fontSize: "1.3rem" },
        },
        h4: {
            fontSize: "1.3rem",
            "@media (max-width:600px)": { fontSize: "1.1rem" },
        },
        h5: {
            fontSize: "1.1rem",
            "@media (max-width:600px)": { fontSize: "1rem" },
        },
        h6: {
            fontSize: "1rem",
            "@media (max-width:600px)": { fontSize: "0.9rem" },
        },
        body1: {
            fontSize: "0.875rem",
            "@media (max-width:600px)": { fontSize: "0.75rem" },
        },
        body2: {
            fontSize: "0.75rem",
            "@media (max-width:600px)": { fontSize: "0.7rem" },
        },
    },
});

export default theme;
