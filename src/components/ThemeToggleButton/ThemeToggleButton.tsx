import React, {ReactElement} from "react";
import MoonIcon from "../../assets/MoonIcon.tsx";
import SunIcon from "../../assets/SunIcon.tsx";
import styles from "./ThemeToggleButton.module.scss";

interface ThemeToggleButtonProps {
    theme: string,
    updateTheme: (theme: string) => void
}

const ThemeToggleButton: React.FC<ThemeToggleButtonProps> = ({ theme, updateTheme }) => {

    let ariaLabel: string;
    let icon: ReactElement;
    let onClick: () => void;

    if (theme === "light-mode") {
        ariaLabel = "Switch to dark mode";
        icon = <SunIcon color={"var(--font-color)"}/>;
        onClick = () => { updateTheme("dark-mode"); };
    } else {
        ariaLabel = "Switch to light mode";
        icon = <MoonIcon color={"var(--font-color)"}/>;
        onClick = () => { updateTheme("light-mode"); };
    }

    return (
        <button className={styles.themeToggleButton} onClick={onClick}
                aria-label={ariaLabel}>
            {icon}
        </button>
    );
};

export default ThemeToggleButton;