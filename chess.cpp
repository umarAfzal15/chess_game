#include <iostream>
#include <cstdio>
#include <string>
#include "raylib.h"

class Board {
private:
    // data
    int grid[8][8];
    Texture2D textures[27];
    int squareSize, startX, startY;
 
    // drag state
    bool isMousePressed;
    int takeRow, takeCol;   // square the piece was picked up from (-1 = nothing picked)
    int turn = 2;
 
    // helpers used only inside the class
    void initGrid();
    void loadTextures();
    bool isInsideBoard(int x, int y) const;
    bool pawnRule(int row, int col, int color) const;
    bool knightRule(int row, int col, int color) const;
    bool rookRule(int row, int col, int color) const;
    bool bishopRule(int row, int col, int color) const;
    bool queenRule(int row, int col, int color) const;
    bool kingRule(int row, int col, int color) const;
 
public:
    Board(int startX, int startY, int squareSize);
    ~Board();
 
    void setTurn(int pieceColor);
    void draw() const;           // draws the squares
    void handleInput();          // mouse pick up / drop logic
    void drawPieces() const;     // draws the pieces (and the one being dragged)
    void drawDebugInfo() const;  // your X, Y, col, row text
    bool isValidMove(int row, int col) const;
};

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

Board::~Board(){
    for (int i = 0; i < 27; i++) {
        if (textures[i].id != 0) UnloadTexture(textures[i]);
    }
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

bool Board::isInsideBoard(int x, int y) const {
    return x >= startX && x < startX + 8 * squareSize &&
           y >= startY && y < startY + 8 * squareSize;
}

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

bool Board::isValidMove(int row, int col) const {

    if (takeRow == -1 || takeCol == -1) return false;

    int pieceColor =  grid[takeRow][takeCol]/10;
    int pieceType = grid[takeRow][takeCol]%10;

    if(pieceColor != turn){
        return false;
    }

    //bool return1;
    if(pieceType == 1){
        return pawnRule(row, col, pieceColor);
    }
    else if(pieceType == 2){
        return knightRule(row, col, pieceColor);
    }
    else if(pieceType == 3){
        return bishopRule(row, col, pieceColor);
    }

    return false;
}

void Board::setTurn(int pieceColor){
    turn = (pieceColor == 1) ? 2 : 1;
}

bool Board::pawnRule(int row, int col, int color) const {
    if(color == 1){
        if((takeRow == 1) && ((col == takeCol) && (row == takeRow + 1 || row == takeRow + 2)) && (grid[row][col] == 0)){

            return true;
        }
        else if((takeRow != 1) && ((col == takeCol)) && (row == takeRow + 1) && (grid[row][col] == 0)){
            return true;
        }
        else if(((row == takeRow + 1) && ((col == takeCol - 1) || col == takeCol + 1)) && grid[row][col] != 0){
            if(color != (grid[row][col]/10)){
                return true;
            }
        }
        else {
            return false;
        }
    }else if(color == 2){
        if((takeRow == 6) && ((col == takeCol) && (row == takeRow - 1 || row == takeRow - 2)) && (grid[row][col] == 0)){
            return true;
        }
        else if((takeRow != 6) && ((col == takeCol)) && (row == takeRow - 1) && (grid[row][col] == 0)){
            return true;
        }
        else if(((row == takeRow - 1) && ((col == takeCol - 1) || col == takeCol + 1)) && grid[row][col] != 0){
            if(color != (grid[row][col]/10)){
                return true;
            }
        }
        else {
            return false;
        }
    }else{
        return false;
    }

    return false;
}
 
bool Board::knightRule(int row, int col, int color) const {

    if((row >= 0 && row <= 7) && (col >= 0 && col <= 7) && 
        (((row == takeRow + 2) && (col == takeCol -1 || col == takeCol +1))||
        ((row == takeRow - 2) && (col == takeCol - 1 || col == takeCol + 1)) ||
        ((col == takeCol - 2) && (row == takeRow - 1 || row == takeRow + 1)) || 
        ((col == takeCol + 2) && (row == takeRow - 1 || row == takeRow + 1)) ))
        {
        
        return true;

    }
    else if(((row == takeRow + 1) && ((col == takeCol - 1) || col == takeCol + 1)) && grid[row][col] != 0){
        if(color != (grid[row][col]/10)){
            return true;
        }
    }
    else {
        return false;
    }

    return false;
}

bool Board::bishopRule(int row, int col, int color) const {

    int rowDef = row - takeRow;
    int colDef = col - takeCol;

    if((abs(rowDef) != abs(colDef)) || rowDef == 0){
        return false;
    }

    int stepRow = (rowDef > 0) ? 1 : -1;
    int stepCol = (colDef > 0) ? 1 : -1;

    int currentRow;
    int currentCol;

    for(int step = 1; step < abs(rowDef); step++){
        currentRow = takeRow + step * stepRow;
        currentCol = takeCol + step * stepCol;

        if(grid[currentRow][currentCol] != 0){
            return false;
        }
    }

    if((grid[currentRow + 1][currentCol + 1] != 0) && (color == grid[currentRow + 1][currentCol + 1]/10)){
        return true;
    }

    return true;
}


int main(){

    InitWindow(1000, 800, "Chess");
    SetTargetFPS(60);
    {
        Board board(180, 80, 80);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(GetColor(0x262421FF));

            board.handleInput();
            board.draw();
            board.drawPieces();
            board.drawDebugInfo();

            EndDrawing();
        }

    }
    CloseWindow();

    return 0;
}