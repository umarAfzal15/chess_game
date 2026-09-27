#include <iostream>
#include <cstdio>
#include <format>
#include <string>
#include "raylib.h"

using namespace std;

void initializeGrid(int chessGrid[8][8]);
void drawBorad(int startX, int startY, int squareSize);
void loadPieceTextures(Texture2D pieceTextures[27]);
void placePieces(int chessGrid[8][8], Texture2D pieceTextures[27], int startX, int startY, int squareSize, int &isMousePressed, int &takeRow, int &takeCol, int &changeRow, int &changeCol);
bool rulesForPieces(int takeRow, int takeCol, int row, int col, int chessGrid[8][8]);
bool pawnRule(int takeRow, int takeCol, int row, int col, int pieceColor);
bool knightRule(int takeRow, int takeCol, int row, int col, int pieceColor);
void unloadPieceTextures(Texture2D pieceTextures[27]);

int main() {

    InitWindow(1000, 800, "Chess");
    SetTargetFPS(60);

    //int size = 8;
    int chessGrid[8][8] = {0};
    
    int isMousePressed = 0;
    int takeRow = -1, takeCol = -1;
    int changeRow = -1, changeCol = -1;

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
        placePieces(chessGrid, pieceTextures, startX, startY, squareSize, isMousePressed, takeRow, takeCol, changeRow, changeCol);

        int mouseX = GetMouseX(); // Get mouse position X
        int mouseY = GetMouseY();

        int col;
        int row;
        col = (mouseX - startX)/squareSize;
        row = (mouseY - startY)/squareSize;

        DrawText(TextFormat("X: %d, Y: %d, col: %d, row: %d", mouseX, mouseY, col, row), 20, 20, 30, GetColor(0xFFFFFFFF));

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

void placePieces(int chessGrid[8][8], Texture2D pieceTextures[27], int startX, int startY, int squareSize,
                  int &isMousePressed, int &takeRow, int &takeCol, int &changeRow, int &changeCol){

    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int col = -1, row = -1;

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

        if(isMousePressed == 0){
            isMousePressed = 1;
            takeCol = (mouseX - startX)/squareSize;
            takeRow = (mouseY - startY)/squareSize;
        }
    }

    col = (mouseX - startX)/squareSize;
    row = (mouseY - startY)/squareSize;

    
    if((takeRow != -1) && (takeCol != -1) && rulesForPieces(takeRow, takeCol, row, col, chessGrid)){
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){

            isMousePressed = 0;

            if((col >= 0 && col <= 7) && (row >= 0 && row <= 7) && (mouseX > 180 && mouseX < 820) && (mouseY > 80 && mouseY < 720)){
                int temp = chessGrid[row][col];
                chessGrid[row][col] = chessGrid[takeRow][takeCol];
                chessGrid[takeRow][takeCol] = temp;
            }

            takeRow = -1;
            takeCol = -1;
        }
    }else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        isMousePressed = 0;
        takeRow = -1;
        takeCol = -1;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            int currentX, currentY;

            if(i == takeRow && j == takeCol){
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

bool rulesForPieces(int takeRow, int takeCol, int row, int col, int chessGrid[8][8]){
    int pieceColor =  chessGrid[takeRow][takeCol]/10;
    int pieceType = chessGrid[takeRow][takeCol]%10;
    //bool return1;
    if(pieceType == 1){
        return pawnRule(takeRow, takeCol, row, col, pieceColor);
    }
    else if(pieceType == 2){
        return knightRule(takeRow, takeCol, row, col, pieceColor);
    }

    return false;
}

bool pawnRule(int takeRow, int takeCol, int row, int col, int pieceColor){

    if(pieceColor == 1){
        if((takeRow == 1) && ((col == takeCol) && (row == takeRow + 1 || row == takeRow + 2))){
            return true;
        }
        else if((takeRow != 1) && ((col == takeCol)) && (row == takeRow + 1)){
            return true;
        }
        else {
            return false;
        }
    }else if(pieceColor == 2){
        if((takeRow == 6) && ((col == takeCol) && (row == takeRow - 1 || row == takeRow - 2))){
            return true;
        }
        else if((takeRow != 6) && ((col == takeCol)) && (row == takeRow - 1)){
            return true;
        }
        else {
            return false;
        }
    }else{
        return false;
    }

    // int diff = row - takeRow;
    // if(diff < 0) diff = -diff; // absolute value, so direction doesn't matter yet

    // if(takeRow == 1 || takeRow == 6){
    //     // starting row — allow 1 or 2 squares
    //     return (diff == 1 || diff == 2);
    // } else {
    //     // any other row — only 1 square
    //     return (diff == 1);
    // }
}

bool knightRule(int takeRow, int takeCol, int row, int col, int pieceColor){

    if(pieceColor == 1){
        if((row >= 0 && row <= 7) && (col >= 0 && col <= 7) && 
            (((row == takeRow + 2) && (col == takeCol -1 || col == takeCol +1))||
            ((row == takeRow - 2) && (col == takeCol - 1 || col == takeCol + 1)) ||
            ((col == takeCol - 2) && (row == takeRow - 1 || row == takeRow + 1)) || 
            ((col == takeCol + 2) && (row == takeRow - 1 || row == takeRow + 1)) ))
            {
            
            return true;

        }else {
            return false;
        }
    }else if(pieceColor == 2){
        if((row >= 0 && row <= 7) && (col >= 0 && col <= 7) && 
            (((row == takeRow - 2) && (col == takeCol -1 || col == takeCol +1))||
            ((row == takeRow + 2) && (col == takeCol - 1 || col == takeCol + 1)) ||
            ((col == takeCol + 2) && (row == takeRow - 1 || row == takeRow + 1)) || 
            ((col == takeCol - 2) && (row == takeRow - 1 || row == takeRow + 1)) ))
            {
            
            return true;

        }else {
            return false;
        }
    }

    return false;
}

void unloadPieceTextures(Texture2D pieceTextures[27]) {
    for (int i = 0; i < 27; i++) {
        if (pieceTextures[i].id != 0) UnloadTexture(pieceTextures[i]);
    }
}