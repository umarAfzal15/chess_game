#include "raylib.h"
#include "board.h"

void Board::handleInput() {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int col = (mouseX - startX) / squareSize;
    int row = (mouseY - startY) / squareSize;

    if(promoting){

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

            int menuY = (promoColor == 2) ? (startY - squareSize) : (startY + 8*squareSize);

            if(mouseX >= startX && mouseY >= menuY && mouseY < menuY + squareSize){

                int slot = col - promoStart;
                int types[4] = {5, 4, 3, 2};

                if(slot >= 0 && slot < 4){
                    grid[promoRow][promoCol] = promoColor*10 + types[slot];
                    promoting = false;

                    int nextColor = (promoColor == 1) ? 2 : 1;

                    if(checkMate(promoColor)){
                        if(isCheck(nextColor)){
                            checkmate = true;
                        }
                        else {
                            stalemate = true;
                        }
                    }
                }
            }
        }

        return;
    }

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

                if((pieceColor == 2 && row == 0 && grid[row][col] == 21) ||
                   (pieceColor == 1 && row == 7 && grid[row][col] == 11)){

                    promoting = true;
                    promoRow = row;
                    promoCol = col;
                    promoColor = pieceColor;

                    promoStart = col - 2;
                    if(promoStart < 0){
                        promoStart = 0;
                    }
                    if(promoStart > 4){
                        promoStart = 4;
                    }
                }
                else if(checkMate(pieceColor)){
                    if(isCheck(nextColor)){
                        checkmate = true;
                    }
                    else {
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