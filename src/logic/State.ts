import {MoveType} from "./Move.ts";
import {MoveSet} from "./MoveSet.ts";

export interface State {
    verticalFences: bigint;  // Needs 64 bit precision
    horizontalFences: bigint;  // Needs 64 bit precision
    playerOneRow: number;
    playerOneCol: number;
    playerTwoRow: number;
    playerTwoCol: number;
    playerOneFenceCount: number;
    playerTwoFenceCount: number;
    playerToMove: number;
    legalMoves: MoveSet;
}

export function isFencePresent(state: State, row: number, col: number, moveType: MoveType): boolean {
    return moveType == MoveType.VerticalFence && (state.verticalFences & (1n << BigInt(row * 8 + col))) != 0n
        || moveType == MoveType.HorizontalFence && (state.horizontalFences & (1n << BigInt(row * 8 + col))) != 0n;
}