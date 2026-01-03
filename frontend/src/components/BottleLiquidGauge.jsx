import React from "react";

export default function BottleLiquidGauge({
                                              width = 200,
                                              height = 380,
                                              percent = 0,
                                          }) {
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
                fill="#1E3A5F"   // 👈 darker text
                style={{
                    fontFamily:
                        "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Inter, sans-serif",
                }}
            >
                {clamped}%
            </text>
        </svg>
    );
}
