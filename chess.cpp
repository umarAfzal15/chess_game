#include <iostream>
#include <cstdio>
#include <format>
#include <string>
#include "raylib.h"

using namespace std;

void initializeGrid(int chessGrid[8][8]);
void drawBorad(int startX, int startY, int squareSize);
void loadPieceTextures(Texture2D pieceTextures[27]);
void placePieces(int chessGrid[8][8], Texture2D pieceTextures[27], int startX, int startY, int squareSize, int row, int col, int mouseX, int mouseY);
void unloadPieceTextures(Texture2D pieceTextures[27]);

int main() {

    InitWindow(1000, 800, "Chess");
    SetTargetFPS(60);

    //int size = 8;
    int chessGrid[8][8] = {0};
    int isMousePressed  = 0;

    Texture2D pieceTextures[27] = {0};
    loadPieceTextures(pieceTextures);

    initializeGrid(chessGrid);

    // Grid Settings
    const int squareSize = 80;
    const int startX = 180; // Top-left starting X offset
    const int startY = 80; // Top-left starting Y offset

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x262421FF));


        drawBorad(startX, startY, squareSize);
        
        if(!IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                isMousePressed = 0;
            }

            placePieces(chessGrid, pieceTextures, startX, startY, squareSize, -1, -1, -1, -1);
        }else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

            int mouseX = GetMouseX(); // Get mouse position X
            int mouseY = GetMouseY();

            DrawText(TextFormat("X: %d, Y: %d", mouseX, mouseY), 20, 20, 30, GetColor(0xFFFFFFFF));

            int col;
            int row;
            if(isMousePressed == 0){
                isMousePressed = 1;
                col = (mouseX - startX)/squareSize;
                row = (mouseY - startY)/squareSize;
            }
            
            placePieces(chessGrid, pieceTextures, startX, startY, squareSize, row, col, mouseX, mouseY);
        }

        EndDrawing();
    }

    unloadPieceTextures(pieceTextures);
    CloseWindow();

    return 0;
}


void initializeGrid(int chessGrid[8][8]){
    
    chessGrid[0][0] = 14;
    chessGrid[0][1] = 12;
    chessGrid[0][2] = 13;
    chessGrid[0][3] = 15;
    chessGrid[0][4] = 16;
    chessGrid[0][5] = 13;
    chessGrid[0][6] = 12;
    chessGrid[0][7] = 14;

    chessGrid[7][0] = 24;
    chessGrid[7][1] = 22;
    chessGrid[7][2] = 23;
    chessGrid[7][3] = 25;
    chessGrid[7][4] = 26;
    chessGrid[7][5] = 23;
    chessGrid[7][6] = 22;
    chessGrid[7][7] = 24;

    for(int i = 1; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(i == 1){
                chessGrid[i][j] = 11;
            }
            else if(i == 6){
                chessGrid[i][j] = 21;
            }
        }
    }
}

void drawBorad(int startX, int startY, int squareSize){

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
}

void loadPieceTextures(Texture2D pieceTextures[27]) {
    int pieceCodes[] = {11,12,13,14,15,16, 21,22,23,24,25,26};
    int numPieces = 12;

    for (int i = 0; i < numPieces; i++) {
        int code = pieceCodes[i];
        Image image = LoadImage(TextFormat("assets/%d.png", code));
        ImageResizeNN(&image, 80, 80);
        pieceTextures[code] = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}

void placePieces(int chessGrid[8][8], Texture2D pieceTextures[27], int startX, int startY, int squareSize, int row, int col, int mouseX, int mouseY){

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            int currentX, currentY;

            if(i == row && j == col){
                currentX = mouseX - 40;
                currentY = mouseY - 40;
            } else {
                currentX = startX + (j * squareSize);
                currentY = startY + (i * squareSize);
            }

            int piece = chessGrid[i][j];
            if(piece != 0){
                DrawTexture(pieceTextures[piece], currentX, currentY, WHITE);
            }
        }
    }
}

void unloadPieceTextures(Texture2D pieceTextures[27]) {
    for (int i = 0; i < 27; i++) {
        if (pieceTextures[i].id != 0) UnloadTexture(pieceTextures[i]);
    }
}