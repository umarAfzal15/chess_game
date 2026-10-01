#include "raylib.h"
#include "board.h"

void Board::handleInput() {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int col = (mouseX - startX) / squareSize;
    int row = (mouseY - startY) / squareSize;
    
    // mouse just pressed: remember which square we picked up
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !isMousePressed) {
        isMousePressed = true;
        if (isInsideBoard(mouseX, mouseY)) {
            takeCol = col;
            takeRow = row;
        }
    }
 
    // mouse released: drop the piece if the move is valid
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (takeRow != -1 && takeCol != -1 &&
            isInsideBoard(mouseX, mouseY) &&
            isValidMove(row, col)) {

            int pieceColor = grid[takeRow][takeCol] / 10;
            int fromRow = takeRow, fromCol = takeCol;

            int captured = grid[row][col];
            grid[row][col] = grid[fromRow][fromCol];
            grid[fromRow][fromCol] = 0;

            if (isCheck(pieceColor)) {
                grid[fromRow][fromCol] = grid[row][col];
                grid[row][col] = captured;
            } else {
                setTurn(pieceColor);

                int nextColor = (pieceColor == 1) ? 2 : 1;

                if(checkMate(pieceColor)){        // opponent has no legal move
                    if(isCheck(nextColor)){       // ...and their king is attacked
                        checkmate = true;
                    }
                    else {                        // ...and their king is NOT attacked
                        stalemate = true;
                    }
                }
            }

        }

        isMousePressed = false;
        takeRow = -1;
        takeCol = -1;
    }

}
