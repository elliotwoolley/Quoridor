import React from "react";
import ArrowLeftDoubleIcon from "../../assets/ArrowLeftDoubleIcon.tsx";
import ArrowLeftIcon from "../../assets/ArrowLeftIcon.tsx";
import ArrowRightDoubleIcon from "../../assets/ArrowRightDoubleIcon.tsx";
import ArrowRightIcon from "../../assets/ArrowRightIcon.tsx";
import Player from "../../logic/Player.ts";
import NewGameModal from "../Modal/NewGameModal";
import styles from "./GameBoardControls.module.scss";

interface GameBoardControlsProps {
    unMakeMove: () => void;
    reMakeMove: () => void;
    unmakeAllMoves: () => void;
    remakeAllMoves: () => void;
    enableForwards: boolean;
    enableBackwards: boolean;
    moves: Array<string>;
    selectedMoveIndex: number;
    onMoveClick: (moveIndex: number) => void;
    newGame: (player1: Player, player2: Player) => void;
    player1: Player;
    player2: Player;
}

const GameBoardControls: React.FC<GameBoardControlsProps> = (
    {
        unMakeMove,
        reMakeMove,
        enableForwards,
        enableBackwards,
        unmakeAllMoves,
        remakeAllMoves,
        moves,
        selectedMoveIndex,
        onMoveClick,
        newGame,
        player1,
        player2,
    }) => (
    <div className={styles.gameBoardControls}>
        <div className={styles.gameBoardControls__controls}>
            <div className={styles.gameBoardControls__header}>Moves</div>
            <div className={styles.gameBoardControls__moves}>
                {Array.from(Array(Math.round(moves.length / 2))).map((_, i) => (
                    <React.Fragment key={`moves-row-${i}`}>
                        <div className={styles.gameBoardControls__movesNumber}>
                            {i + 1}.
                        </div>
                        <div
                            className={`
                                ${styles.gameBoardControls__movesItem} 
                                ${2 * i == selectedMoveIndex && styles.gameBoardControls__movesItemSelected}`}
                            onClick={() => {
                                onMoveClick(2 * i);
                            }}>
                            {moves[2 * i]}
                        </div>
                        <div
                            className={`
                                ${styles.gameBoardControls__movesItem} 
                                ${2 * i + 1 == selectedMoveIndex && styles.gameBoardControls__movesItemSelected}`}
                            onClick={() => {
                                onMoveClick(2 * i + 1);
                            }}>
                            {moves[2 * i + 1]}
                        </div>
                    </React.Fragment>
                ))}
            </div>
            <div className={styles.gameBoardControls__buttonFooter}>
                <button className={styles.controlButton} onClick={unmakeAllMoves} disabled={!enableBackwards}>
                    <ArrowLeftDoubleIcon/>
                </button>
                <button className={styles.controlButton} onClick={unMakeMove} disabled={!enableBackwards}>
                    <ArrowLeftIcon/>
                </button>
                <button className={styles.controlButton} onClick={reMakeMove} disabled={!enableForwards}>
                    <ArrowRightIcon/>
                </button>
                <button className={styles.controlButton} onClick={remakeAllMoves} disabled={!enableForwards}>
                    <ArrowRightDoubleIcon/>
                </button>
            </div>
        </div>
        <div className={styles.gameBoardControls__newGame}>
            <NewGameModal onStartGame={newGame} player1={player1} player2={player2}/>
        </div>
    </div>
);

export default GameBoardControls;
