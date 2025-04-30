import React from "react";
import {Move} from "../../logic/Move.ts";
import Pawn from "../Pawn/Pawn.tsx";
import styles from "./BoardSquare.module.scss";

interface BoardSquareProps {
    move: Move;
    isLegal: boolean;
    tryMakeMove: (move: Move) => void;
    player: number | null;
}

const BoardSquare: React.FC<BoardSquareProps> = ({ move, isLegal, tryMakeMove, player }) => {
    const rowClass = styles[`row-${move.row}`];
    const colClass = styles[`col-${move.col}`];

    const handleClick = isLegal ? () => tryMakeMove(move) : undefined;

    const squareClasses = `${styles.boardSquare} ${rowClass} ${colClass} ${isLegal ? styles.legal : ""}`;

    return (
        <div className={squareClasses} onClick={handleClick}>
            {player && (
                <Pawn color={`var(--player${player}-color)`} size="60%"/>
            )}
            {isLegal && <div className={styles.moveIndicator}/>}
        </div>
    );
};

export default BoardSquare;