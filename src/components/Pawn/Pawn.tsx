import React from "react";

interface PawnProps {
    color: string;
    size: number | string | undefined;
}

const Pawn: React.FC<PawnProps> = ({ color, size }) => {
    return (
        <div style={{
            height: 0,
            width: size,
            paddingBottom: size
        }}>
            <svg
                xmlns="http://www.w3.org/2000/svg"
                width={"100%"}
                viewBox="0 0 90 90"
                fill="none">
                <circle cx={45} cy={20} r={20} fill={color}/>
                <ellipse cx={45} cy={90} rx={30} ry={60} fill={color}/>
            </svg>
        </div>
    );
};

export default Pawn;