import React from "react";
import {Move, MoveType} from "../../logic/Move";
import styles from "./Fence.module.scss";

interface FenceProps {
    move: Move;
    isLegal: boolean;
    tryMakeMove: (move: Move) => void;
    isFencePresent: boolean;
}

const Fence: React.FC<FenceProps> = ({ move, isLegal, tryMakeMove, isFencePresent }) => {
    let fenceClass = styles.fence;
    let positionClass = "";

    if (isFencePresent) {
        fenceClass += ` ${styles.fencePresent}`;
    }

    if (isLegal) {
        fenceClass += ` ${styles.fenceLegal}`;
    }

    if (move.moveType === MoveType.HorizontalFence) {
        fenceClass += ` ${styles.fenceHorizontal}`;
        positionClass = `${styles["horizontal-row-" + move.row]} ${styles["horizontal-col-" + move.col]}`;
    } else if (move.moveType === MoveType.VerticalFence) {
        fenceClass += ` ${styles.fenceVertical}`;
        positionClass = `${styles["vertical-row-" + move.row]} ${styles["vertical-col-" + move.col]}`;
    }

    const handleClick = isLegal ? () => tryMakeMove(move) : undefined;
    const classes = `${fenceClass} ${positionClass}`;

    return (
        <div className={classes} onClick={handleClick}/>
    );
};

export default Fence;