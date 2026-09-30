#include "raylib.h"
#include "board.h"

Board::Board(int startX, int startY, int squareSize) {
    this->startX = startX;
    this->startY = startY;
    this->squareSize = squareSize;
 
    isMousePressed = false;
    takeRow = -1;
    takeCol = -1;
 
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = 0;
 
    for (int i = 0; i < 27; i++)
        textures[i] = Texture2D{};
 
    initGrid();
    loadTextures();
}

void Board::initGrid(){

    grid[0][0] = 14;
    grid[0][1] = 12;
    grid[0][2] = 13;
    grid[0][3] = 15;
    grid[0][4] = 16;
    grid[0][5] = 13;
    grid[0][6] = 12;
    grid[0][7] = 14;

    grid[7][0] = 24;
    grid[7][1] = 22;
    grid[7][2] = 23;
    grid[7][3] = 25;
    grid[7][4] = 26;
    grid[7][5] = 23;
    grid[7][6] = 22;
    grid[7][7] = 24;

    for(int i = 1; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(i == 1){
                grid[i][j] = 11;
            }
            else if(i == 6){
                grid[i][j] = 21;
            }
        }
    }

}

void Board::loadTextures() {
    int pieceCodes[] = {11,12,13,14,15,16, 21,22,23,24,25,26};
    int numPieces = 12;

    for (int i = 0; i < numPieces; i++) {
        int code = pieceCodes[i];
        Image image = LoadImage(TextFormat("assets/%d.png", code));
        ImageResizeNN(&image, squareSize, squareSize);
        textures[code] = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}