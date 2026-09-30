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