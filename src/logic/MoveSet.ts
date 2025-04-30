import {Move, MoveType} from "./Move.ts";

export class MoveSet {
    private readonly moves: boolean[];

    public constructor() {
        this.moves = new Array<boolean>(81 * 3);  // 81 board squares, 3 move types
        this.moves.fill(false);
    }

    public add(move: Move): void {
        this.moves[this.hash(move)] = true;
    }

    public contains(move: Move): boolean {
        if (move.moveType === MoveType.None) {
            return false;
        }
        return this.moves[this.hash(move)];
    }

    private hash(move: Move): number {
        return (move.moveType - 1) * 81 + move.row * 9 + move.col % 9;
    }
}