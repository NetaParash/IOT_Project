
import React, { useMemo } from "react";

const WaterLevelClassroomDashboard = React.memo(function WaterLevel({
                                                      percent,
                                                      width = 140,
                                                      height = 260,
                                                  }) {
    const clamped = Math.max(0, Math.min(percent, 100));

    const fillColor = useMemo(() => {
        const r = Math.round(255 * (100 - clamped) / 100);
        const g = Math.round(180 * clamped / 100);
        return `rgb(${r}, ${g}, 80)`; // red → green
    }, [clamped]);

    const neckTop = 10;
    const neckBottom = 60;
    const bodyBottom = height - 10;

    const bodyHeight = bodyBottom - neckBottom;
    const waterHeight = (clamped / 100) * bodyHeight;
    const waterY = bodyBottom - waterHeight;

    return (
        <svg width={width} height={height} viewBox={`0 0 ${width} ${height}`}>
            <defs>
                <clipPath id={`bottle-clip-${percent}`}>
                    <path
                        d={`
                          M ${width * 0.42} ${neckTop}
                          L ${width * 0.58} ${neckTop}
                          L ${width * 0.58} ${neckBottom}
                          Q ${width * 0.58} ${neckBottom + 20} ${width * 0.7} ${neckBottom + 20}
                          L ${width * 0.7} ${bodyBottom}
                          L ${width * 0.3} ${bodyBottom}
                          L ${width * 0.3} ${neckBottom + 20}
                          Q ${width * 0.42} ${neckBottom + 20} ${width * 0.42} ${neckBottom}
                          Z
                        `}
                    />
                </clipPath>
            </defs>

            {/* Fill */}
            <rect
                x={0}
                y={waterY}
                width={width}
                height={waterHeight}
                fill={fillColor}
                clipPath={`url(#bottle-clip-${percent})`}
                style={{ transition: "all 0.6s ease" }}
            />

            {/* Outline */}
            <path
                d={`
                  M ${width * 0.42} ${neckTop}
                  L ${width * 0.58} ${neckTop}
                  L ${width * 0.58} ${neckBottom}
                  Q ${width * 0.58} ${neckBottom + 20} ${width * 0.7} ${neckBottom + 20}
                  L ${width * 0.7} ${bodyBottom}
                  L ${width * 0.3} ${bodyBottom}
                  L ${width * 0.3} ${neckBottom + 20}
                  Q ${width * 0.42} ${neckBottom + 20} ${width * 0.42} ${neckBottom}
                  Z
                `}
                fill="none"
                stroke="#3A7BD5"
                strokeWidth="4"
            />

            {/* Percentage text */}
            <text
                x="50%"
                y={neckBottom + bodyHeight / 2}
                textAnchor="middle"
                dominantBaseline="middle"
                fontSize="20"
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

export default WaterLevelClassroomDashboard;
