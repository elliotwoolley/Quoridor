// TODO : We need to not import this everytime we instantiate it. (Move to WasmUtils)

import {Move} from "../logic/Move";
import {State} from "../logic/State";
import WasmUtils from "../logic/WasmUtils.ts";

interface AiWorkerData {
    gameState: State;
    difficulty: number;
}

addEventListener("message", (event: MessageEvent<AiWorkerData>) => {
    const { gameState, difficulty } = event.data;
    const move: Move = WasmUtils.getAiMove(gameState, difficulty);
    postMessage({ type: "result", move });
});

postMessage({ type: "ready" });
