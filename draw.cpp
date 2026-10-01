#include <string>
#include "raylib.h"
#include "board.h"


void Board::draw() const {
    Color lightSquare = GetColor(0xEEEEEEFF); // Added alpha byte (FF) for safety
    Color darkSquare  = GetColor(0x7086B8FF);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            
            // Calculate exact pixel position for this specific square
            int currentX = startX + (col * squareSize);
            int currentY = startY + (row * squareSize);

            // Alternates colors by checking if row + col is even or odd
            Color squareColor;
            if ((row + col) % 2 == 0) {
                squareColor = lightSquare;
            } else {
                squareColor = darkSquare;
            }

            DrawRectangle(currentX, currentY, squareSize, squareSize, squareColor);
        }
    }

    if(promoting){
        int types[4] = {5, 4, 3, 2};
        int menuY = (promoColor == 2) ? (startY - squareSize) : (startY + 8*squareSize);

        for(int j = 0; j < 4; j++){
            int currentX = startX + ((promoStart + j) * squareSize);

            DrawTexture(textures[promoColor*10 + types[j]], currentX, menuY, WHITE);
        }
    }
}

void Board::drawPieces() const {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
 
    int draggedPiece = 0;
    int draggedX = 0;
    int draggedY = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = grid[i][j];
            if (piece == 0) continue;
 
            int currentX, currentY;
            if (i == takeRow && j == takeCol) {
                // piece being dragged follows the mouse
                currentX = mouseX - squareSize / 2;
                currentY = mouseY - squareSize / 2;
            } else {
                currentX = startX + j * squareSize;
                currentY = startY + i * squareSize;
            }
 
            if (i == takeRow && j == takeCol) {
                draggedX = currentX;
                draggedY = currentY;
                draggedPiece = piece;
                continue;
            }

            DrawTexture(textures[piece], currentX, currentY, WHITE);
        }
    }

    if (takeRow != -1 && takeCol != -1) {
        DrawTexture(textures[draggedPiece], draggedX, draggedY, WHITE);
    }
}

void Board::drawDebugInfo() const {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int col = (mouseX - startX) / squareSize;
    int row = (mouseY - startY) / squareSize;
 
    DrawText(TextFormat("X: %d, Y: %d, col: %d, row: %d", mouseX, mouseY, col, row),
             20, 20, 30, WHITE);
}