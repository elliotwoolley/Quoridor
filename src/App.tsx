import {useEffect, useState} from "react";
import "./styles/main.scss";
import GameBoard from "./components/GameBoard/GameBoard.tsx";
import ThemeToggleButton from "./components/ThemeToggleButton/ThemeToggleButton.tsx";

export default function App() {

    const [darkMode, setDarkMode] = useState(() => {
        return window.matchMedia && window.matchMedia("(prefers-color-scheme: dark)").matches;
    });

    useEffect(() => {
        if (darkMode) {
            document.body.classList.add("dark-mode");
        } else {
            document.body.classList.remove("dark-mode");
        }
    }, [darkMode]);

    return (
        <div className={`app ${darkMode ? "dark-mode" : "light-mode"}`}>
            <ThemeToggleButton darkMode={darkMode} setDarkMode={setDarkMode}/>
            <h1 className="title">QUORIDOR</h1>
            <GameBoard/>
        </div>
    );
}
