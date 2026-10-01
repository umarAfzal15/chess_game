# Chess Game — Technical Documentation

A two-player desktop chess game written in C++ with [raylib](https://www.raylib.com/). Two players share one mouse and move pieces by dragging and dropping them. The game enforces the complete rules of chess: normal piece movement, check, pins, checkmate, stalemate, castling, en passant and pawn promotion.

---

## Table of Contents

1. [Features](#1-features)
2. [Tech Stack and Build](#2-tech-stack-and-build)
3. [Project Structure](#3-project-structure)
4. [Board Representation](#4-board-representation)
5. [The `Board` Class](#5-the-board-class)
6. [Game Flow](#6-game-flow)
7. [Piece Movement Rules](#7-piece-movement-rules)
8. [Special Moves](#8-special-moves)
9. [Check Detection](#9-check-detection)
10. [Checkmate and Stalemate](#10-checkmate-and-stalemate)
11. [Rendering](#11-rendering)
12. [Design Conventions](#12-design-conventions)
13. [Testing Checklist](#13-testing-checklist)
14. [Troubleshooting](#14-troubleshooting)
15. [Possible Improvements](#15-possible-improvements)

---

## 1. Features

- Drag-and-drop piece movement with the mouse
- Turn-based play with White moving first
- Movement rules for all six piece types
- Capturing, with blocked-path detection for bishops, rooks and queens
- Check detection and **pin handling**: a move that leaves your own king attacked is rejected
- Checkmate and stalemate detection
- **Castling** (kingside and queenside)
- **En passant** capture
- **Pawn promotion** with an on-screen piece-selection menu
- Textures loaded once at startup from the `assets/` folder

---

## 2. Tech Stack and Build

| Item | Details |
|---|---|
| Language | C++ (C++11 or newer, because of default member initializers) |
| Graphics, window, input | raylib |
| Compiler | g++ from MSYS2 MinGW-w64 |
| Assets | PNG images in `assets/`, named by piece code (for example `assets/25.png`) |

### Build command

Every `.cpp` file must be part of the build. Compiling only the current file produces linker errors (see [Troubleshooting](#14-troubleshooting)).

```bash
g++ *.cpp -o chess -lraylib -lopengl32 -lgdi32 -lwinmm
```

If raylib was installed through MSYS2, it can be installed with:

```bash
pacman -S mingw-w64-x86_64-raylib
```

### VS Code `tasks.json`

Use all source files instead of only the open one:

```json
"args": [
    "-g",
    "${workspaceFolder}/*.cpp",
    "-o",
    "${workspaceFolder}/chess.exe",
    "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm"
]
```

---

## 3. Project Structure

The game is built around one class, `Board`, declared in `board.h`. Its member functions are split across several `.cpp` files by responsibility.

| Responsibility | Functions |
|---|---|
| Class declaration | `board.h` |
| Input handling | `Board::handleInput` (`handle_input.cpp`) |
| Move validation and rules | `isValidMove`, `setTurn`, `pawnRule`, `knightRule`, `bishopRule`, `rookRule`, `queenRule`, `kingRule` |
| Check detection | `isCheck` |
| Checkmate and stalemate | `checkMate` |
| Drawing | Board, pieces, promotion menu, game-over message |
| Entry point | `main.cpp`: window setup, game loop |
| Assets | `assets/<code>.png` |

---

## 4. Board Representation

### The grid

The board is an `int grid[8][8]`. Each square holds a **piece code**:

```
code = color * 10 + type        (0 = empty square)
```

| Value | Meaning |
|---|---|
| `color` 1 | Black (top of the screen) |
| `color` 2 | White (bottom of the screen) |

| `type` | Piece |
|---|---|
| 1 | Pawn |
| 2 | Knight |
| 3 | Bishop |
| 4 | Rook |
| 5 | Queen |
| 6 | King |

Examples: `11` is a black pawn, `25` is a white queen, `16` is the black king, `26` is the white king.

Useful decoding:

```cpp
int pieceColor = grid[row][col] / 10;
int pieceType  = grid[row][col] % 10;
```

### Orientation

- `row 0` is the **top** of the screen and `row 7` is the **bottom**. `col 0` is the left edge.
- Black starts on rows 0 and 1 and its pawns move toward higher rows (**+1**).
- White starts on rows 6 and 7 and its pawns move toward lower rows (**-1**).

### Starting position

```
        col: 0   1   2   3   4   5   6   7
row 0 (B):  14  12  13  15  16  13  12  14
row 1 (B):  11  11  11  11  11  11  11  11
row 2:       0   0   0   0   0   0   0   0
row 3:       0   0   0   0   0   0   0   0
row 4:       0   0   0   0   0   0   0   0
row 5:       0   0   0   0   0   0   0   0
row 6 (W):  21  21  21  21  21  21  21  21
row 7 (W):  24  22  23  25  26  23  22  24
```

### Screen to board conversion

```cpp
int col = (mouseX - startX) / squareSize;
int row = (mouseY - startY) / squareSize;
```

`startX` and `startY` are the top-left corner of the board on the screen, and `squareSize` is the side of one square in pixels. `isInsideBoard(mouseX, mouseY)` must be checked before using `row` and `col`.

---

## 5. The `Board` Class

### Members

| Member | Type | Purpose |
|---|---|---|
| `grid[8][8]` | `int` | The board; see [Board Representation](#4-board-representation) |
| `textures[27]` | `Texture2D` | One texture per piece code (`textures[25]` is the white queen). Loaded once at startup |
| `squareSize`, `startX`, `startY` | `int` | Board geometry on screen |
| `isMousePressed` | `bool` | True while the left button is held, so a pick-up happens only once |
| `takeRow`, `takeCol` | `int` | Square the piece was picked up from (`-1` when nothing is picked up) |
| `turn` | `int` | Color that must move next (`2` = White starts) |
| `checkmate`, `stalemate` | `bool` | Game-over flags, both `false` at start |
| `promoting` | `bool` | True while the promotion menu is open |
| `promoRow`, `promoCol` | `int` | Square of the pawn being promoted |
| `promoColor` | `int` | Color of the pawn being promoted |
| `promoStart` | `int` | First column of the promotion menu |
| `enPassantRow`, `enPassantCol` | `int` | Square skipped by a pawn's double step on the previous move (`-1` when no en passant capture is available) |
| `kingMoved[3]` | `bool` | Whether the king of each color has moved (index 1 = Black, 2 = White) |
| `rookMoved[3][2]` | `bool` | Whether each color's rook has moved (second index 0 = queenside rook, 1 = kingside rook) |

### Public interface

| Function | Description |
|---|---|
| `handleInput()` | Reads the mouse and applies moves; call once per frame |
| `draw()` | Draws the board, pieces, drag preview, promotion menu and game-over text |

### Important shared state: `takeRow` / `takeCol`

The rule functions (`pawnRule`, `knightRule`, ...) do **not** receive the source square as a parameter. They read it from `takeRow` and `takeCol`. Any function that tests moves for a piece other than the dragged one (`isCheck`, `checkMate`) must:

1. Save `takeRow` and `takeCol` at the start,
2. Set them to the square of the piece being tested,
3. Restore them before returning.

---

## 6. Game Flow

`handleInput()` runs every frame and works in three phases.

### Phase 1: Promotion menu (if open)

If `promoting` is true, only the promotion menu is handled and the function returns. See [Pawn promotion](#pawn-promotion).

### Phase 2: Pick up

On the first frame the left button is down (`IsMouseButtonDown && !isMousePressed`):

- `isMousePressed` is set to true.
- If the cursor is inside the board, `takeRow` and `takeCol` store the square.

### Phase 3: Drop

On `IsMouseButtonReleased`, if a square was picked up, the cursor is on the board, and `isValidMove(row, col)` is true:

```
1. Remember the source square (fromRow, fromCol) and the piece color.
2. Make the move on the grid, remembering any captured piece.
   - Castling: also move the rook.
   - En passant: also remove the captured pawn.
3. Call isCheck(pieceColor).
     true  -> the move exposes the player's own king: undo everything.
     false -> the move is legal:
         a. Update castling flags and the en passant square.
         b. setTurn(pieceColor) switches the turn.
         c. If a pawn reached the last row: open the promotion menu (promoting = true).
            Otherwise: run the checkmate / stalemate test (see section 10).
4. Reset isMousePressed, takeRow and takeCol.
```

The move is **tried first and undone if illegal**. This one test covers pins, moving the king into an attacked square, and ignoring a check.

---

## 7. Piece Movement Rules

Every rule function has the same signature:

```cpp
bool Board::xxxRule(int row, int col, int color) const;
```

- `(row, col)` is the **target** square, `color` is the mover's color, and the source square comes from `takeRow` / `takeCol`.
- The result is `true` if the piece may move there by its own rules. King safety is checked separately by `isCheck`.
- A target holding a piece of the **same color** is always rejected. A target holding an enemy piece is a capture.

`isValidMove(row, col)` is the dispatcher. It returns `false` if nothing is picked up or if the picked-up piece does not belong to the player whose `turn` it is, and otherwise calls the rule for the piece type.

### Pawn (`pawnRule`)

| Direction | White (color 2) | Black (color 1) |
|---|---|---|
| Forward | row − 1 | row + 1 |
| Start row | 6 | 1 |

- **One step forward** onto an empty square.
- **Two steps forward** from the start row, only if both squares in front of the pawn are empty.
- **Diagonal capture** one square forward onto a square holding an enemy piece.
- **En passant** and **promotion**: see [Special Moves](#8-special-moves).
- A pawn can never capture straight ahead or move diagonally onto an empty square (except en passant).

### Knight (`knightRule`)

- Moves in an "L": 2 squares in one direction and 1 square perpendicular (8 possible targets).
- Jumps over pieces.
- Target must be on the board and not hold a piece of its own color.

### Bishop (`bishopRule`)

- Moves any distance diagonally: `|rowDiff| == |colDiff|` and `rowDiff != 0`.
- Every square **between** the source and target must be empty.
- Target must be empty or hold an enemy piece.

### Rook (`rookRule`)

- Moves any distance along a row or a column (exactly one of `row == takeRow` or `col == takeCol`).
- Every square between the source and target must be empty.
- Target must be empty or hold an enemy piece.

### Queen (`queenRule`)

- Combines the two: legal if `bishopRule` or `rookRule` accepts the move.

### King (`kingRule`)

- Moves one square in any of the eight directions.
- Target must be empty or hold an enemy piece.
- Also accepts **castling** (see [Castling](#castling)).

---

## 8. Special Moves

### Pawn promotion

**Trigger:** a pawn reaches the opposite end of the board. White pawns promote on `row 0` (code `21`), black pawns on `row 7` (code `11`).

**Flow:**

1. The move is accepted (it passed the `isCheck` test). The pawn stays on the last row.
2. `promoting` is set to true and `promoRow`, `promoCol`, `promoColor` store the pawn's square and color.
3. `promoStart` is set to `col - 2`, clamped to the range `0..4` so that the four choices never leave the board.
4. `draw()` shows four pieces in a row: **Queen, Rook, Bishop, Knight** (types 5, 4, 3, 2). The menu appears **above the board for White** (`startY - squareSize`) and **below the board for Black** (`startY + 8 * squareSize`).
5. While `promoting` is true, `handleInput()` ignores everything except a click on one of the four choices. The clicked slot is `col - promoStart`.
6. The pawn is replaced with `promoColor * 10 + chosenType`, `promoting` becomes false, and the checkmate / stalemate test runs for the first time. The test is intentionally delayed until the piece is chosen, because the new piece may give check or mate.

### En passant

**Rule:** immediately after an enemy pawn advances **two squares** from its start row, a pawn of yours standing **beside** it (same row, adjacent column) may capture it as if it had advanced only one square. This right exists for **one move only**.

**State:** `enPassantRow` and `enPassantCol` hold the square the pawn skipped. They are set after every double step and reset to `-1` after any other move.

| Double step by | Lands on | Skipped square (en passant target) | Capturing pawn must stand on |
|---|---|---|---|
| Black (from row 1) | row 3 | row 2 | row 3 (a white pawn) |
| White (from row 6) | row 4 | row 5 | row 4 (a black pawn) |

**Rules:**

- `pawnRule` accepts a diagonal move onto the **empty** en passant target square.
- When the move is made, the captured pawn, which is **not** on the target square but on the source row at the target column (`grid[fromRow][col]`), is removed from the board.
- The capture is tested for king safety like any other move: both pawns are moved off their squares in the simulation before `isCheck` runs. This catches the rare case where removing both pawns from the same row exposes the king to a rook or queen.
- `checkMate` counts en passant as a legal move.

### Castling

**Rule:** the king moves **two squares** toward a rook and the rook jumps to the square the king crossed.

| Side | King | Rook |
|---|---|---|
| Kingside | col 4 to col 6 | col 7 to col 5 |
| Queenside | col 4 to col 2 | col 0 to col 3 |

The king's row is `row 7` for White and `row 0` for Black.

**Conditions (all must hold):**

1. The king has never moved (`kingMoved[color]` is false).
2. The rook involved has never moved (`rookMoved[color][side]` is false) and is still on its starting square.
3. All squares between the king and the rook are empty (for queenside: columns 1, 2, 3; for kingside: columns 5, 6).
4. The king is **not in check**.
5. The king does **not pass through** an attacked square (col 5 for kingside, col 3 for queenside).
6. The king does **not land on** an attacked square.

**Implementation notes:**

- `kingRule` accepts the two-square king move when conditions 1-3 hold. Conditions 4-6 are tested with `isCheck`, by temporarily placing the king on each square it crosses.
- `handleInput` moves the rook as part of the same move, and undoes both pieces if the final `isCheck` test fails.
- `kingMoved` and `rookMoved` are updated after every accepted move. A rook that is **captured** on its starting square also loses its castling right because the square no longer holds a rook.
- Castling is never a capture, so it is ignored by [check detection](#9-check-detection).

---

## 9. Check Detection

```cpp
bool Board::isCheck(int color);
```

Returns `true` if the **king of `color` is currently attacked**.

**Algorithm:**

1. Save `takeRow` and `takeCol`.
2. Find the king (`color * 10 + 6`) on the grid. If there is none, return `false`.
3. For every piece on the board that is **not** empty and **not** of `color`:
   - Set `takeRow` and `takeCol` to that piece's square.
   - Ask its rule function whether it can move onto the king's square.
   - Stop as soon as one piece attacks the king.
4. Restore `takeRow` and `takeCol` and return the result.

**Why it works:** the rule functions allow a piece to "capture" a king, so asking "can this enemy piece move onto my king's square?" is exactly asking "does this piece attack my king?". Such a capture never happens in play, because any move that leaves your own king attacked is rejected first.

Castling and en passant cannot capture a king, so they do not affect this function.

---

## 10. Checkmate and Stalemate

```cpp
bool Board::checkMate(int color);
```

> **Naming note:** pieces of `color` are skipped, so the function tests the moves of the **opposite** side. It is called with the color of the player who has just moved, and it asks whether the opponent can move at all.

**Return value:** `true` when the tested side has **no legal move**. It returns `false` when at least one legal move exists.

**Algorithm:** for every piece of the tested side and every target square `(k, l)`:

1. Check the move with the piece's rule function.
2. If the move is allowed, play it on the grid, remembering any captured piece.
3. Call `isCheck` for the tested side.
4. Undo the move.
5. If the king was **not** attacked, the move is legal: stop searching, the answer is `false`.

If no move survives step 5, the function returns `true`.

**Distinguishing checkmate from stalemate** (in `handleInput`, after a move):

```cpp
int nextColor = (pieceColor == 1) ? 2 : 1;

if(checkMate(pieceColor)){          // opponent has no legal move
    if(isCheck(nextColor)){         // ...and their king is attacked
        checkmate = true;
    }
    else {                          // ...and their king is NOT attacked
        stalemate = true;
    }
}
```

| Opponent has a legal move? | Opponent's king attacked? | Result |
|---|---|---|
| Yes | either | Game continues |
| No | Yes | **Checkmate** |
| No | No | **Stalemate** (draw) |

When either flag is true, `handleInput()` stops accepting moves and `draw()` shows the result.

---

## 11. Rendering

- **Textures** are loaded **once at startup** with `LoadImage`, `ImageResizeNN(&image, squareSize, squareSize)`, `LoadTextureFromImage` and `UnloadImage`, and stored in `textures[code]`. Loading must never happen inside the per-frame draw or input code.
- **Board:** 64 alternating light and dark squares starting at `(startX, startY)`.
- **Pieces:** for every non-zero `grid[row][col]`, draw `textures[grid[row][col]]` at `startX + col * squareSize`, `startY + row * squareSize`. The piece currently being dragged is drawn at the cursor instead of its square.
- **Promotion menu:** drawn while `promoting` is true, using `textures[promoColor * 10 + type]` for the types `{5, 4, 3, 2}`.
- **Game over:** when `checkmate` or `stalemate` is true, a message such as `"Checkmate!"` or `"Stalemate!"` is drawn over the board.

---

## 12. Design Conventions

- **Piece codes are the single source of truth.** Color and type are always derived from `/ 10` and `% 10`.
- **Try, test, undo.** Any test of "is this move legal?" is made by changing the grid, calling `isCheck`, and restoring the grid exactly. Saving the captured piece before the move is mandatory.
- **Rule functions only answer geometry and blocking.** King safety belongs to `isCheck`, never to the rule functions.
- **Save and restore `takeRow` / `takeCol`** in every function that changes them.
- **Early exit with flags.** Nested loops stop with `break` chains as soon as a result is known.
- **Update order after a legal move:** castling and en passant bookkeeping, then `setTurn`, then promotion or the mate test.

---

## 13. Testing Checklist

| Scenario | Moves / setup | Expected result |
|---|---|---|
| Fool's mate | 1. f3 e5 2. g4 Qh4 | `checkmate = true` after Qh4 |
| Scholar's mate | 1. e4 e5 2. Bc4 Nc6 3. Qh5 Nf6 4. Qxf7 | `checkmate = true` after Qxf7 |
| Illegal knight move | Drag a knight one square diagonally | Move rejected |
| Pin | Move a pinned piece off the line of attack | Move rejected |
| King into check | Move the king onto an attacked square | Move rejected |
| Escaping check | Move a different piece while in check | Rejected unless it resolves the check |
| Stalemate | Black king a8, white king c7, white queen b6, Black to move | `stalemate = true` |
| Promotion | Walk a pawn to the last row | Menu appears, chosen piece replaces the pawn |
| Promotion mate | Promote to a piece that delivers mate | `checkmate = true` after the choice |
| Pawn double step | Place a piece directly in front of an unmoved pawn | Neither step allowed |
| En passant | Double-step a pawn beside an enemy pawn | Capture available immediately, gone one move later |
| En passant pin | Capture would expose the king along the row | Capture rejected |
| Kingside castling | Clear f and g squares, nothing attacked | King on col 6, rook on col 5 |
| Queenside castling | Clear b, c and d squares | King on col 2, rook on col 3 |
| Castling out of check | King attacked | Castling rejected |
| Castling through check | Attacked square between king and target | Castling rejected |
| Castling after moving | Move the king or rook away and back | Castling rejected |
| No input after game over | Click after mate or stalemate | Nothing moves |

---

## 14. Troubleshooting

### Linker error: `undefined reference to Board::something(...)`

```
ld.exe: ... handle_input.cpp:(.text+0x168): undefined reference to `Board::checkMate(int)'
collect2.exe: error: ld returned 1 exit status
```

This is a **linker** error, not a compiler error. The code compiled, but the linker could not find the function body. Reading it:

- The file named in the message is where the function is **used**, not where the problem is.
- `` `Board::checkMate(int)` `` is the exact signature the linker is searching for.

Check, in this order:

1. **The `.cpp` file defining the function is not part of the build.** Compile all files (`g++ *.cpp ...`). This is the most common cause.
2. **The function is declared but never defined.** Search for `Board::functionName`.
3. **Declaration and definition differ**: name, parameter types, or a missing or extra `const`.
4. **The definition is commented out** or inside an `#ifdef`.

### The game behaves as before after a fix

If the build failed, g++ does not create a new `.exe`, and the old one still runs. Delete the old `.exe`, rebuild, confirm there are no errors, and run the new file.

### Crash or garbage values on start

- Initialize `isMousePressed = false`, and `takeRow` / `takeCol = -1`.
- Never read `grid[takeRow][takeCol]` before a piece has been picked up. Compute the piece color only inside the drop block.

### Memory growth or slowdown

Textures are being loaded every frame. Load them once at startup and only call `DrawTexture` while playing.

---

## 15. Possible Improvements

- Other draws: insufficient material, threefold repetition, fifty-move rule
- Highlight legal target squares for the picked-up piece
- Move history in algebraic notation, with undo
- Move and capture sounds
- Pawn promotion by keyboard shortcut
- Restart button after the game ends
- A chess clock
- A computer opponent (minimax with alpha-beta pruning would reuse the existing `isCheck` and move tests)