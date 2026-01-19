// src/components/BottleDropdown.js
import React from "react";
import { Autocomplete, TextField } from "@mui/material";
import { useAppContext } from "../AppContext";

export default function BottleDropdown() {
    const {
        bottles,
        selectedBottleId,
        setSelectedBottleId,
    } = useAppContext();

    return (
        <Autocomplete
            disablePortal
            options={bottles}
            loading={!bottles || bottles.length === 0}

            getOptionLabel={(option) => option.name}

            // ✅ controlled value from context
            value={bottles.find(b => b.id === selectedBottleId) || null}

            onChange={(e, newBottle) =>
                setSelectedBottleId(newBottle ? newBottle.id : null)
            }

            isOptionEqualToValue={(option, value) =>
                option.id === value?.id
            }

            sx={{
                width: 350,
                mb: 3,
                mt: 1,
                "& .MuiInputBase-root": {
                    height: 48,
                    fontSize: 20,
                },
                "& .MuiInputLabel-root": {
                    fontSize: 18,
                }
            }}

            renderInput={(params) => (
                <TextField
                    {...params}
                    label="Select Bottle"
                    size="medium"
                />
            )}
        />
    );
}
