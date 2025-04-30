import React, {useEffect, useState} from "react";
import {Move, moveToString, MoveType} from "../../logic/Move.ts";
import Player from "../../logic/Player.ts";
import {isFencePresent, State} from "../../logic/State.ts";
import WasmUtils from "../../logic/WasmUtils.ts";
import BoardSquare from "../BoardSquare/BoardSquare.tsx";
import ConfettiEffect from "../ConfettiEffect.tsx";
import Fence from "../Fence/Fence.tsx";
import GameBoardControls from "../GameBoardControls/GameBoardControls.tsx";
import Pawn from "../Pawn/Pawn.tsx";
import styles from "./GameBoard.module.scss";

interface PlayerInfoProps {
    player: Player;
    playerNumber: number;
    fenceCount: number;
}

const PlayerInfo: React.FC<PlayerInfoProps> = ({ player, playerNumber, fenceCount }) => {
    return (
        <div className={styles.gameBoard__playerInfo}>
            <div className={styles.gameBoard__playerInfoGap}/>
            <Pawn color={`var(--player${playerNumber}-color)`} size={"60%"}/>
            <div className={styles.gameBoard__playerType}>
                {player.type}
                {player.type === "AI" && ` (Lvl ${player.difficulty})`}
            </div>
            <div className={styles.gameBoard__fenceCount}>{fenceCount}</div>
            <div className={styles.gameBoard__fenceCountContainer}>
                {Array(fenceCount).fill(0).map((_, i) => (
                    <div key={`p${playerNumber}-fence-${i}`} className={styles.gameBoard__fence}/>
                ))}
            </div>
        </div>
    );
};

interface BoardGameProps {
    player1?: Player;
    player2?: Player;
}

const GameBoard: React.FC<BoardGameProps> = ({ player1, player2 }) => {
    const [gameState, setGameState] = useState<State>(WasmUtils.newState());
    const [moveList, setMoveList] = useState<Array<Move>>([]);
    const [moveIndex, setMoveIndex] = useState<number>(-1);
    const [player1State, setPlayer1State] = useState<Player>(player1 ?? { type: "Human", difficulty: 3 });
    const [player2State, setPlayer2State] = useState<Player>(player2 ?? { type: "AI", difficulty: 4 });
    const [gameOver, setGameOver] = useState<boolean>(false);
    const [winningPlayer, setWinningPlayer] = useState<number | undefined>(undefined);
    const [aiIsThinking, setAiIsThinking] = useState<boolean>(false);

    // Win check and AI Move
    useEffect(() => {
        if (gameOver) {
            return;
        }

        if (WasmUtils.winCheck(gameState)) {
            setGameOver(true);
            setWinningPlayer(gameState.playerToMove === 1 ? 2 : 1);
            return;
        }

        const currentPlayer = gameState.playerToMove === 1 ? player1State : player2State;

        if (currentPlayer.type === "AI" && !aiIsThinking && moveIndex === moveList.length - 1) {
            const worker = new Worker(new URL("../../workers/AiWorker.ts", import.meta.url), { type: "module" });
            setAiIsThinking(true);

            worker.postMessage({
                gameState: gameState,
                difficulty: currentPlayer.difficulty!,
            });

            worker.onmessage = (event: MessageEvent<any>) => {
                if (event.data.type === "ready") {
                    worker.postMessage({
                        type: "calculate",
                        gameState: gameState,
                        difficulty: currentPlayer.difficulty!,
                    });
                } else if (event.data.type === "result") {
                    const move: Move = event.data.move;
                    const newMoveList = [...moveList, move];

                    setGameState(WasmUtils.makeMove(gameState, move));
                    setMoveList(newMoveList);
                    setMoveIndex(moveIndex + 1);
                    setAiIsThinking(false);
                    worker.terminate();
                }
            };
        }
    }, [gameState, aiIsThinking, gameOver, moveList, player1State, player2State, moveIndex]);

    function tryMakeMove(move: Move) {
        if (gameState.legalMoves.contains(move) && !gameOver) {
            move.fromRow = gameState.playerToMove === 1 ? gameState.playerOneRow : gameState.playerTwoRow;
            move.fromCol = gameState.playerToMove === 1 ? gameState.playerOneCol : gameState.playerTwoCol;

            let newMoveList = moveList;
            if (moveIndex !== moveList.length - 1) {
                newMoveList = moveList.slice(0, moveIndex + 1);
            }
            newMoveList = [...newMoveList, move];

            setGameState(WasmUtils.makeMove(gameState, move));
            setMoveList(newMoveList);
            setMoveIndex(moveIndex + 1);
        }
    }

    function goToMove(targetIndex: number) {
        let newGameState = gameState;
        if (targetIndex > moveIndex) {
            for (let i = moveIndex + 1; i <= targetIndex; i++) {
                newGameState = WasmUtils.makeMove(newGameState, moveList[i]);
            }
        } else {
            for (let i = moveIndex; i > targetIndex; i--) {
                newGameState = WasmUtils.unMakeMove(newGameState, moveList[i]);
            }
        }
        setGameState(newGameState);
        setMoveIndex(targetIndex);
    }

    function unmakeMove() {
        goToMove(moveIndex - 1);
    }

    function remakeMove() {
        goToMove(moveIndex + 1);
    }

    function unmakeAllMoves() {
        goToMove(-1);
    }

    function remakeAllMoves() {
        goToMove(moveList.length - 1);
    }

    function newGame(newPlayer1: Player, newPlayer2: Player) {
        setGameState(WasmUtils.newState());
        setMoveList([]);
        setMoveIndex(-1);
        setPlayer1State(newPlayer1);
        setPlayer2State(newPlayer2);
        setGameOver(false);
        setWinningPlayer(undefined);
        setAiIsThinking(false);
    }

    function determinePlayer(squareRow: number, squareCol: number): number | null {
        if (gameState.playerOneRow === squareRow && gameState.playerOneCol === squareCol) {
            return 1;
        }
        if (gameState.playerTwoRow === squareRow && gameState.playerTwoCol === squareCol) {
            return 2;
        }
        return null;
    }

    const isHumanTurn = (gameState.playerToMove === 1 ? player1State : player2State).type === "Human";
    const playerCanMakeMove = isHumanTurn && !gameOver;
    const enableForwards = !aiIsThinking && moveIndex < moveList.length - 1;
    const enableBackwards = !aiIsThinking && moveIndex >= 0;

    return (
        <>
            {gameOver && (
                <ConfettiEffect
                    color={getComputedStyle(document.documentElement).getPropertyValue(`--player${winningPlayer}-color`)}
                />
            )}
            <div className={styles.gameBoard__container}>
                <PlayerInfo player={player1State} playerNumber={1} fenceCount={gameState.playerOneFenceCount}/>
                <div className={styles.gameBoard__grid}>
                    {Array.from(Array(9)).map((_, row) =>
                        Array.from(Array(9)).map((_, col) => (
                            <BoardSquare
                                key={`board-square-${row}-${col}`}
                                move={{ moveType: MoveType.Pawn, row, col }}
                                isLegal={
                                    playerCanMakeMove &&
                                    gameState.legalMoves.contains({ moveType: MoveType.Pawn, row, col })
                                }
                                tryMakeMove={tryMakeMove}
                                player={determinePlayer(row, col)}
                            />
                        ))
                    )}
                    {Array.from(Array(8)).map((_, row) =>
                        Array.from(Array(8)).map((_, col) => (
                            <React.Fragment key={`fences-${row}-${col}`}>
                                <Fence
                                    key={`fence-horizontal-${row}-${col}`}
                                    move={{ moveType: MoveType.HorizontalFence, row, col }}
                                    isLegal={
                                        playerCanMakeMove &&
                                        gameState.legalMoves.contains({ moveType: MoveType.HorizontalFence, row, col })
                                    }
                                    tryMakeMove={tryMakeMove}
                                    isFencePresent={isFencePresent(gameState, row, col, MoveType.HorizontalFence)}
                                />
                                <Fence
                                    key={`fence-vertical-${row}-${col}`}
                                    move={{ moveType: MoveType.VerticalFence, row, col }}
                                    isLegal={
                                        playerCanMakeMove &&
                                        gameState.legalMoves.contains({ moveType: MoveType.VerticalFence, row, col })
                                    }
                                    tryMakeMove={tryMakeMove}
                                    isFencePresent={isFencePresent(gameState, row, col, MoveType.VerticalFence)}
                                />
                            </React.Fragment>
                        ))
                    )}
                </div>
                <PlayerInfo player={player2State} playerNumber={2} fenceCount={gameState.playerTwoFenceCount}/>
                <GameBoardControls
                    unMakeMove={unmakeMove}
                    reMakeMove={remakeMove}
                    unmakeAllMoves={unmakeAllMoves}
                    remakeAllMoves={remakeAllMoves}
                    enableForwards={enableForwards}
                    enableBackwards={enableBackwards}
                    moves={moveList.map(m => moveToString(m))}
                    selectedMoveIndex={moveIndex}
                    onMoveClick={goToMove}
                    newGame={newGame}
                    player1={player1State}
                    player2={player2State}
                />
            </div>
        </>
    );
};

export default GameBoard;