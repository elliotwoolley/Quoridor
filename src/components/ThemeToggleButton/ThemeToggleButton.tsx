import React from "react";
import MoonIcon from "../../assets/MoonIcon.tsx";
import SunIcon from "../../assets/SunIcon.tsx";
import styles from "./ThemeToggleButton.module.scss";

interface ThemeToggleButtonProps {
    setDarkMode: (darkMode: boolean) => void,
    darkMode: boolean
}

const ThemeToggleButton: React.FC<ThemeToggleButtonProps> = ({ setDarkMode, darkMode }) => (
    <button className={styles.themeToggleButton} onClick={() => setDarkMode(!darkMode)}
            aria-label={darkMode ? "Switch to light mode" : "Switch to dark mode"}>
        {darkMode ? <MoonIcon color={"var(--font-color)"}/> : <SunIcon color={"var(--font-color)"}/>}
    </button>
);

export default ThemeToggleButton;