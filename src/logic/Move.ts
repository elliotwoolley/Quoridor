export enum FenceType {
    Vertical = 1,
    Horizontal = 2,
}

export enum MoveType {
    None = 0,
    VerticalFence = FenceType.Vertical,
    HorizontalFence = FenceType.Horizontal,
    Pawn = 3,
}

export interface Move {
    moveType: MoveType;
    row: number;
    col: number;
    fromRow?: number;
    fromCol?: number;
}

export function moveToString(move: Move): string {
    let row: number = (9 - move.row);
    const col: string = String.fromCharCode("a".charCodeAt(0) + move.col);
    let fence: string = "";

    if (move.moveType === MoveType.VerticalFence) {
        fence = "v";
        row--;
    } else if (move.moveType === MoveType.HorizontalFence) {
        fence = "h";
        row--;
    }

    return row.toString() + col + fence;
}