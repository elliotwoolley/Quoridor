import React, {useState} from "react";
import GameControllerIcon from "../../assets/GameControllerIcon.tsx";
import Player from "../../logic/Player";
import styles from "./NewGameModal.module.scss";

interface NewGameModalProps {
    player1: Player;
    player2: Player;
    onStartGame: (player1: Player, player2: Player) => void;
}

const minDifficulty = 2;
const maxDifficulty = 6;
const medDifficulty = Math.round((maxDifficulty + minDifficulty) / 2);

const difficultyMarks = [
    { value: minDifficulty, label: "Easy" },
    { value: medDifficulty, label: "Medium" },
    { value: maxDifficulty, label: "Hard" }
];

interface PlayerConfigProps {
    className: string;
    label: string;
    playerType: "Human" | "AI";
    onTypeChange: (value: "Human" | "AI") => void;
    difficulty: number;
    onDifficultyChange: (value: number) => void;
}

const PlayerConfig: React.FC<PlayerConfigProps> = (
    {
        className,
        label,
        playerType,
        onTypeChange,
        difficulty,
        onDifficultyChange
    }) => {
    return (
        <div className={`${styles.playerConfig} ${className}`}>
            <div className={styles.playerConfigLabel}>{label}</div>
            <div className={styles.radioGroup}>
                <label>
                    <input
                        type="radio"
                        name={label}
                        value="Human"
                        checked={playerType === "Human"}
                        onChange={() => onTypeChange("Human")}
                    />
                    Human
                </label>
                <label>
                    <input
                        type="radio"
                        name={label}
                        value="AI"
                        checked={playerType === "AI"}
                        onChange={() => onTypeChange("AI")}
                    />
                    AI
                </label>
            </div>
            <div className={styles.sliderContainer} style={{ opacity: playerType === "AI" ? 1 : 0.5 }}>
                <div className={styles.sliderLabel}>AI Difficulty</div>
                <input
                    type="range"
                    min={minDifficulty}
                    max={maxDifficulty}
                    step={1}
                    value={difficulty}
                    disabled={playerType !== "AI"}
                    onChange={(e) => onDifficultyChange(Number(e.target.value))}
                />
                <div className={styles.sliderMarks}>
                    {difficultyMarks.map(mark => (
                        <span key={mark.value}>{mark.label}</span>
                    ))}
                </div>
            </div>
        </div>
    );
};

const NewGameModal: React.FC<NewGameModalProps> = (props) => {
    const [open, setOpen] = useState(false);
    const [player1, setPlayer1] = useState<Player>(props.player1);
    const [player2, setPlayer2] = useState<Player>(props.player2);

    const handleOpen = () => setOpen(true);
    const handleClose = () => setOpen(false);
    const handleStartGame = () => {
        props.onStartGame(player1, player2);
        handleClose();
    };

    return (
        <div className={styles.newGameModal}>
            <button className="primaryButton" onClick={handleOpen}>
                <GameControllerIcon color="var(--primary-button-font-color)"/> New Game
            </button>
            {open && (
                <div className={styles.modalOverlay} onClick={handleClose}>
                    <div className={styles.modalContent} onClick={(e) => e.stopPropagation()}>
                        <button className={styles.modalCloseButton} onClick={handleClose}>
                            &times;
                        </button>
                        <h2 className={styles.modalTitle}>New Game</h2>
                        <div className={styles.modalBody}>
                            <PlayerConfig
                                key={"player-config-1"}
                                className="player1Accent"
                                label="Player 1"
                                playerType={player1.type}
                                onTypeChange={(value) => setPlayer1({ ...player1, type: value })}
                                difficulty={player1.difficulty || medDifficulty}
                                onDifficultyChange={(value) => setPlayer1({ ...player1, difficulty: value })}
                            />
                            <hr className={styles.divider}/>
                            <PlayerConfig
                                key={"player-config-2"}
                                className="player2Accent"
                                label="Player 2"
                                playerType={player2.type}
                                onTypeChange={(value) => setPlayer2({ ...player2, type: value })}
                                difficulty={player2.difficulty || medDifficulty}
                                onDifficultyChange={(value) => setPlayer2({ ...player2, difficulty: value })}
                            />
                        </div>
                        <div className={styles.modalFooter}>
                            <button className="primaryButton" onClick={handleStartGame}>
                                Start Game
                            </button>
                        </div>
                    </div>
                </div>
            )}
        </div>
    );
};

export default NewGameModal;