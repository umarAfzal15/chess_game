#include <iostream>
#include "raylib.h"
#include "board.h"

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
    else if(pieceType == 4){
        return rookRule(row, col, pieceColor);
    }
    else if (pieceType == 5){
        return queenRule(row, col, pieceColor);
    }
    else if(pieceType == 6){
        return kingRule(row, col, pieceColor);
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
            
            int type = (color/2 == 0) ? 1 : 2;

            if((grid[row][col] == (type*10 + 6))){
                return false;
            }
            else if(color != (grid[row][col]/10)){
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

        if(grid[row][col] == 0){
            return true;
        }

        if(color != (grid[row][col]/10)){
            return true;
        }
    }

    return false;
}

bool Board::bishopRule(int row, int col, int color) const {

    int rowDef = row - takeRow;
    int colDef = col - takeCol;

    if (rowDef == 0 || abs(rowDef) != abs(colDef)) {
        return false;
    }

    int stepRow = (rowDef > 0) ? 1 : -1;
    int stepCol = (colDef > 0) ? 1 : -1;

    for (int step = 1; step < abs(rowDef); step++) {
        int currentRow = takeRow + step * stepRow;
        int currentCol = takeCol + step * stepCol;

        if (grid[currentRow][currentCol] != 0) {
            return false;
        }
    }

    int target = grid[row][col];

    if (target == 0) {
        return true;
    }

    if(color != (grid[row][col]/10)){
        return true;
    }
    return false;
}

bool Board::rookRule(int row, int col, int color) const {

    bool sameRow = (row == takeRow);
    bool sameCol = (col == takeCol);

    if (sameRow && sameCol) {
        return false;
    }
    if (!sameRow && !sameCol) {
        return false;
    }

    if (sameRow) {
        int step;
        if (col > takeCol) {
            step = 1;
        } else {
            step = -1;
        }

        for (int c = takeCol + step; c != col; c += step) {
            if (grid[row][c] != 0) {
                return false;
            }
        }
    }
    else {
        int step;
        if (row > takeRow) {
            step = 1;
        } else {
            step = -1;
        }

        for (int r = takeRow + step; r != row; r += step) {
            if (grid[r][col] != 0) {
                return false;
            }
        }
    }

    int target = grid[row][col];

    if (target == 0) {
        return true;
    }
    
    if(color != (grid[row][col]/10)){
        return true;
    }
    return false;
}

bool Board::queenRule(int row, int col, int color) const {

    if(bishopRule(row, col, color) || rookRule(row, col, color)){
        return true;
    }

    return false;
}

bool Board::kingRule(int row, int col, int color) const {
    if (((col == takeCol + 1 || col == takeCol - 1) && row == takeRow) ||
        ((row == takeRow + 1 || row == takeRow - 1) && col == takeCol) ||
        ((col == takeCol - 1 && row == takeRow - 1) || (col == takeCol + 1 && row == takeRow + 1)) ||
        ((col == takeCol + 1 && row == takeRow - 1) || (col == takeCol - 1 && row == takeRow + 1))) {
        
        if(color != (grid[row][col]/10)){
            return true;
        }
    }

    return false;
}