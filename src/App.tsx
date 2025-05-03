import {useState} from "react";
import "./styles/main.scss";
import GameBoard from "./components/GameBoard/GameBoard.tsx";
import ThemeToggleButton from "./components/ThemeToggleButton/ThemeToggleButton.tsx";

export default function App() {

    const [theme, setTheme] = useState(() => {
        return localStorage.getItem('theme') ?? 'dark-mode';
    });

    function updateTheme(theme: string) {
        localStorage.setItem('theme', theme);
        setTheme(theme);
    }

    return (
        <div className={`app ${theme}`}>
            <ThemeToggleButton theme={theme} updateTheme={updateTheme}/>
            <h1 className="title">QUORIDOR</h1>
            <GameBoard/>
        </div>
    );
}
