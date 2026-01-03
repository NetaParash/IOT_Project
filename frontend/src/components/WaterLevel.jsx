import React, { useEffect, useRef, useState } from "react";

const POLL_INTERVAL_MS = 30_000;

const WaterLevel = React.memo(function BottleLevel({
                                                        width = 200,
                                                        height = 380,
                                                    }) {
    const [percent, setPercent] = useState(0);
    const lastPercentRef = useRef(0);

    async function fetchWaterLevel() {
        try {
            const res = await fetch(
                "https://iot-project-6i3k.onrender.com/api/app/water-level"
            );
            const data = await res.json();

            const nextPercent = Math.floor(
                (data.water_level_ml / data.capacity_ml) * 100
            );

            // only update if value actually changed
            if (nextPercent !== lastPercentRef.current) {
                lastPercentRef.current = nextPercent;
                setPercent(nextPercent);
            }
        } catch (err) {
            console.error("Water level fetch failed", err);
        }
    }

    useEffect(() => {
        fetchWaterLevel(); // initial fetch
        const id = setInterval(fetchWaterLevel, POLL_INTERVAL_MS);
        return () => clearInterval(id);
    }, []);

    // ---------- SVG logic ----------
    const clamped = Math.max(0, Math.min(percent, 100));

    const neckTop = 10;
    const neckBottom = 70;
    const bodyBottom = height - 10;

    const bodyHeight = bodyBottom - neckBottom;
    const waterHeight = (clamped / 100) * bodyHeight;
    const waterY = bodyBottom - waterHeight;

    return (
        <svg width={width} height={height} viewBox={`0 0 ${width} ${height}`}>
            <defs>
                <clipPath id="bottle-clip">
                    <path
                        d={`
              M ${width * 0.42} ${neckTop}
              L ${width * 0.58} ${neckTop}
              L ${width * 0.58} ${neckBottom}

              Q ${width * 0.58} ${neckBottom + 25} ${width * 0.7} ${neckBottom + 25}
              L ${width * 0.7} ${bodyBottom}

              L ${width * 0.3} ${bodyBottom}

              L ${width * 0.3} ${neckBottom + 25}
              Q ${width * 0.42} ${neckBottom + 25} ${width * 0.42} ${neckBottom}

              Z
            `}
                    />
                </clipPath>
            </defs>

            {/* Water fill */}
            <rect
                x={0}
                y={waterY}
                width={width}
                height={waterHeight}
                fill="#6EC6FF"
                clipPath="url(#bottle-clip)"
                style={{ transition: "all 0.8s ease" }}
            />

            {/* Bottle outline */}
            <path
                d={`
          M ${width * 0.42} ${neckTop}
          L ${width * 0.58} ${neckTop}
          L ${width * 0.58} ${neckBottom}

          Q ${width * 0.58} ${neckBottom + 25} ${width * 0.7} ${neckBottom + 25}
          L ${width * 0.7} ${bodyBottom}

          L ${width * 0.3} ${bodyBottom}

          L ${width * 0.3} ${neckBottom + 25}
          Q ${width * 0.42} ${neckBottom + 25} ${width * 0.42} ${neckBottom}

          Z
        `}
                fill="none"
                stroke="#3A7BD5"
                strokeWidth="5"
            />

            {/* Percentage text */}
            <text
                x="50%"
                y={neckBottom + bodyHeight / 2}
                textAnchor="middle"
                dominantBaseline="middle"
                fontSize="24"
                fontWeight="600"
                letterSpacing="0.5px"
                fill="#1E3A5F"
                style={{
                    fontFamily:
                        "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Inter, sans-serif",
                }}
            >
                {clamped}%
            </text>
        </svg>
    );
});

export default WaterLevel;
