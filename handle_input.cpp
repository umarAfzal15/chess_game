#include "raylib.h"
#include "board.h"

void Board::handleInput() {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int col = (mouseX - startX) / squareSize;
    int row = (mouseY - startY) / squareSize;
    int pieceColor =  grid[takeRow][takeCol]/10;

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

            if(grid[row][col] != 0){
                grid[row][col] = 0;
            }
 
            int temp = grid[row][col];
            grid[row][col] = grid[takeRow][takeCol];
            grid[takeRow][takeCol] = temp;

            setTurn(pieceColor);
        }
 
        isMousePressed = false;
        takeRow = -1;
        takeCol = -1;
    }
}
