// @ts-expect-error - Generated js file. Declaration file is not needed.
import createModule from "../wasm/Quoridor.js";
import {Move, MoveType} from "./Move.ts";
import {MoveSet} from "./MoveSet.ts";
import {State} from "./State";

const module = await createModule();

export default abstract class WasmUtils {

    public static newState(): State {
        const statePtr = module._new_state_ptr();
        const state = this.readWasmState(statePtr);
        module._free(statePtr);
        return state;
    }

    public static makeMove(state: State, move: Move): State {
        const statePtr = this.stateToWasm(state);
        const movePtr = this.moveToWasm(move);

        module._make_move(statePtr, movePtr);
        state = this.readWasmState(statePtr);

        module._free(statePtr);
        module._free(movePtr);

        return state;
    }

    public static unMakeMove(state: State, move: Move): State {
        const statePtr = this.stateToWasm(state);
        const movePtr = this.moveToWasm(move);

        module._unmake_move(statePtr, movePtr);
        state = this.readWasmState(statePtr);

        module._free(statePtr);
        module._free(movePtr);

        return state;
    }

    public static getAiMove(state: State, depth: number): Move {
        const statePtr = this.stateToWasm(state);
        const movePtr = module._get_move_ptr();
        module._get_best_move_iterative_deepening(movePtr, statePtr, depth);

        const move: Move = this.readWasmMove(movePtr, state);

        module._free(statePtr);
        module._free(movePtr);

        return move;
    }

    public static winCheck(state: State): boolean {
        const statePtr = this.stateToWasm(state);
        const gameIsOver = module._win_check(statePtr);
        module._free(statePtr);
        return gameIsOver;
    }

    private static getLegalMoves(state: State, statePtr: number): MoveSet {
        const pawnMask: number = module._generate_legal_pawn_moves(statePtr);
        const verticalFenceMask: bigint = module._generate_fully_legal_vertical_fence_moves(statePtr);
        const horizontalFenceMask: bigint = module._generate_fully_legal_horizontal_fence_moves(statePtr);

        const pawnRow: number = state.playerToMove == 1 ? state.playerOneRow : state.playerTwoRow;
        const pawnCol: number = state.playerToMove == 1 ? state.playerOneCol : state.playerTwoCol;

        const pawnMoves = this.pawnMovesFromWasm(pawnMask, pawnRow, pawnCol);
        const fenceMoves = this.fenceMovesFromWasm(verticalFenceMask, horizontalFenceMask);

        const moveSet = new MoveSet();

        for (const move of pawnMoves) {
            moveSet.add(move);
        }

        for (const move of fenceMoves) {
            moveSet.add(move);
        }

        return moveSet;
    }

    private static stateToWasm(state: State): number {
        const statePtr: number = module._get_state_ptr();
        let offset = 0;

        module.HEAPU32[(statePtr + offset) >> 2] = Number(state.verticalFences & 0xffffffffn);
        module.HEAPU32[(statePtr + offset + 4) >> 2] = Number(state.verticalFences >> 32n);
        offset += 8;

        module.HEAPU32[(statePtr + offset) >> 2] = Number(state.horizontalFences & 0xffffffffn);
        module.HEAPU32[(statePtr + offset + 4) >> 2] = Number(state.horizontalFences >> 32n);
        offset += 8;

        module.HEAPU8[statePtr + offset++] = state.playerOneRow;
        module.HEAPU8[statePtr + offset++] = state.playerOneCol;
        module.HEAPU8[statePtr + offset++] = state.playerTwoRow;
        module.HEAPU8[statePtr + offset++] = state.playerTwoCol;
        module.HEAPU8[statePtr + offset++] = state.playerOneFenceCount;
        module.HEAPU8[statePtr + offset++] = state.playerTwoFenceCount;
        module.HEAPU8[statePtr + offset++] = state.playerToMove;

        return statePtr;
    }

    private static readWasmState(ptr: number): State {

        let offset = 0;

        let lo = module.HEAPU32[(ptr + offset) >> 2];
        let hi = module.HEAPU32[(ptr + offset + 4) >> 2];
        const verticalFences = (BigInt(hi) << 32n) | BigInt(lo);
        offset += 8;

        lo = module.HEAPU32[(ptr + offset) >> 2];
        hi = module.HEAPU32[(ptr + offset + 4) >> 2];
        const horizontalFences = (BigInt(hi) << 32n) | BigInt(lo);
        offset += 8;

        const playerOneRow = module.HEAPU8[ptr + offset++];
        const playerOneCol = module.HEAPU8[ptr + offset++];
        const playerTwoRow = module.HEAPU8[ptr + offset++];
        const playerTwoCol = module.HEAPU8[ptr + offset++];
        const playerOneFenceCount = module.HEAPU8[ptr + offset++];
        const playerTwoFenceCount = module.HEAPU8[ptr + offset++];
        const playerToMove = module.HEAPU8[ptr + offset++];
        const legalMoves = new MoveSet();

        const state: State = {
            verticalFences,
            horizontalFences,
            playerOneRow,
            playerOneCol,
            playerTwoRow,
            playerTwoCol,
            playerOneFenceCount,
            playerTwoFenceCount,
            playerToMove,
            legalMoves: legalMoves
        };

        state.legalMoves = this.getLegalMoves(state, ptr);

        return state;
    }

    private static moveToWasm(move: Move): number {
        const ptr = module._get_move_ptr();
        let offset = 0;

        if (move.moveType === MoveType.Pawn) {
            const rowDif = move.row - move.fromRow!;
            const colDif = move.col - move.fromCol!;

            const pawnMove: PawnMoveTypes | undefined = PawnMoveBiMap.getMove(rowDif, colDif);

            if (pawnMove === undefined) {
                console.error(`Invalid pawn move: (${rowDif}, ${colDif}).`);
            }

            module.HEAPU32[((ptr + offset) >> 2)] = pawnMove;


        } else if (move.moveType === MoveType.VerticalFence || move.moveType === MoveType.HorizontalFence) {
            const fenceMove: bigint = 1n << BigInt(move.row * 8 + move.col);

            module.HEAPU32[(ptr + offset) >> 2] = Number(fenceMove & 0xffffffffn);
            module.HEAPU32[((ptr + offset) >> 2) + 1] = Number(fenceMove >> 32n);
        } else if (move.moveType === MoveType.None) {
            module.HEAPU32[(ptr + offset) >> 2] = 0;
            module.HEAPU32[((ptr + offset) >> 2) + 1] = 0;
        } else {
            console.error(`Invalid move type: ${move.moveType}.`);
        }
        offset += 8;

        module.HEAPU32[(ptr + offset) >> 2] = move.moveType;
        offset += 4;

        // Score
        module.HEAP32[(ptr + offset) >> 2] = 0;

        return ptr;
    }

    private static readWasmMove(ptr: number, state: State): Move {
        // Read move type first
        let offset = 8;

        const moveType: MoveType = module.HEAPU16[(ptr + offset) >> 1];

        offset = 0;

        switch (moveType) {
            case MoveType.None:
                return { moveType: moveType, row: -1, col: -1 };
            case MoveType.Pawn: {
                const playerRow = state.playerToMove === 1 ? state.playerOneRow : state.playerTwoRow;
                const playerCol = state.playerToMove === 1 ? state.playerOneCol : state.playerTwoCol;

                const pawnMove: PawnMoveTypes = module.HEAPU32[(ptr + offset) >> 2];

                const rowColDif: [number, number] | undefined = PawnMoveBiMap.getDif(pawnMove);

                if (rowColDif === undefined) {
                    throw Error(`Illegal PawnMoveType: ${pawnMove}`);
                }

                return {
                    moveType: moveType,
                    row: playerRow + rowColDif[0],
                    col: playerCol + rowColDif[1],
                    fromRow: playerRow,
                    fromCol: playerCol
                };
            }
            case MoveType.VerticalFence:
            case MoveType.HorizontalFence: {
                const lo = module.HEAPU32[(ptr + offset) >> 2];
                const hi = module.HEAPU32[(ptr + offset + 4) >> 2];

                let leadingZeros = Math.clz32(hi);

                if (leadingZeros === 32) {
                    leadingZeros += Math.clz32(lo);
                }

                const fenceIndex = 63 - leadingZeros;

                return { moveType: moveType, row: Math.floor(fenceIndex / 8), col: fenceIndex % 8 };
            }
        }
    }

    private static pawnMovesFromWasm(pawnMask: number, row: number, col: number): Move[] {

        const moves = new Array<Move>();

        if (PawnMoveTypes.North & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row - 1,
                col: col,
            });
        }

        if (PawnMoveTypes.East & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row,
                col: col + 1,
            });
        }

        if (PawnMoveTypes.South & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row + 1,
                col: col,
            });
        }

        if (PawnMoveTypes.West & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row,
                col: col - 1,
            });
        }

        if (PawnMoveTypes.NorthNorth & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row - 2,
                col: col,
            });
        }

        if (PawnMoveTypes.EastEast & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row,
                col: col + 2,
            });
        }

        if (PawnMoveTypes.SouthSouth & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row + 2,
                col: col,
            });
        }

        if (PawnMoveTypes.WestWest & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row,
                col: col - 2,
            });
        }

        if (PawnMoveTypes.NorthEast & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row - 1,
                col: col + 1,
            });
        }

        if (PawnMoveTypes.NorthWest & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row - 1,
                col: col - 1,
            });
        }

        if (PawnMoveTypes.SouthEast & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row + 1,
                col: col + 1,
            });
        }

        if (PawnMoveTypes.SouthWest & pawnMask) {
            moves.push({
                moveType: MoveType.Pawn,
                row: row + 1,
                col: col - 1,
            });
        }

        return moves;
    }

    private static fenceMovesFromWasm(verticalFenceMask: bigint, horizontalFenceMask: bigint): Move[] {
        const moves = new Array<Move>();

        for (let index: bigint = 0n; index < 64n; index++) {
            const mask: bigint = 1n << index;

            if ((mask & verticalFenceMask) != 0n) {
                moves.push({
                    moveType: MoveType.VerticalFence,
                    row: Math.floor(Number(index / 8n)),
                    col: Number(index % 8n),
                });
            }

            if ((mask & horizontalFenceMask) != 0n) {
                moves.push({
                    moveType: MoveType.HorizontalFence,
                    row: Math.floor(Number(index / 8n)),
                    col: Number(index % 8n),
                });
            }
        }

        return moves;
    }
}

enum PawnMoveTypes {
    North = 0b000000000001,
    East = 0b000000000010,
    South = 0b000000000100,
    West = 0b000000001000,

    NorthNorth = 0b000000010000,
    EastEast = 0b000000100000,
    SouthSouth = 0b000001000000,
    WestWest = 0b000010000000,

    NorthEast = 0b000100000000,
    NorthWest = 0b001000000000,
    SouthEast = 0b010000000000,
    SouthWest = 0b100000000000
}

class PawnMoveBiMap {
    private static readonly entries: [number, number, PawnMoveTypes][] = [
        [-1, 0, PawnMoveTypes.North],
        [0, 1, PawnMoveTypes.East],
        [1, 0, PawnMoveTypes.South],
        [0, -1, PawnMoveTypes.West],
        [-2, 0, PawnMoveTypes.NorthNorth],
        [0, 2, PawnMoveTypes.EastEast],
        [2, 0, PawnMoveTypes.SouthSouth],
        [0, -2, PawnMoveTypes.WestWest],
        [-1, 1, PawnMoveTypes.NorthEast],
        [-1, -1, PawnMoveTypes.NorthWest],
        [1, 1, PawnMoveTypes.SouthEast],
        [1, -1, PawnMoveTypes.SouthWest],
    ];

    public static getMove(rowDif: number, colDif: number): PawnMoveTypes | undefined {
        return this.difToMove.get(this.toKey(rowDif, colDif));
    }

    public static getDif(move: PawnMoveTypes): [number, number] | undefined {
        return this.moveToDif.get(move);
    }

    private static toKey(row: number, col: number): string {
        return `${row},${col}`;
    }

    private static readonly difToMove: ReadonlyMap<string, PawnMoveTypes> = new Map(
        PawnMoveBiMap.entries.map(([row, col, move]) =>
            [PawnMoveBiMap.toKey(row, col), move])
    );

    private static readonly moveToDif: ReadonlyMap<PawnMoveTypes, [number, number]> = new Map(
        PawnMoveBiMap.entries.map(([row, col, move]) =>
            [move, [row, col]])
    );
}
