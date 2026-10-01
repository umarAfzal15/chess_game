#include "raylib.h"
#include "board.h"

bool Board::isCheck(int color) {

    int savedRow = takeRow;
    int savedCol = takeCol;

    int kingRow = -1;
    int kingCol = -1;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(grid[i][j] == (color*10 + 6)){
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if(kingRow != -1){
            break;
        }
    }

    if(kingRow == -1){
        return false;
    }

    bool attacked = false;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            int piece = grid[i][j];

            if(piece == 0 || piece/10 == color){
                continue;
            }

            int enemy = piece/10;
            int pieceType = piece%10;

            takeRow = i;
            takeCol = j;

            if(pieceType == 1){
                attacked = pawnRule(kingRow, kingCol, enemy);
            }
            else if(pieceType == 2){
                attacked = knightRule(kingRow, kingCol, enemy);
            }
            else if(pieceType == 3){
                attacked = bishopRule(kingRow, kingCol, enemy);
            }
            else if(pieceType == 4){
                attacked = rookRule(kingRow, kingCol, enemy);
            }
            else if(pieceType == 5){
                attacked = queenRule(kingRow, kingCol, enemy);
            }
            else if(pieceType == 6){
                attacked = kingRule(kingRow, kingCol, enemy);
            }

            if(attacked){
                break;
            }
        }
        if(attacked){
            break;
        }
    }

    takeRow = savedRow;
    takeCol = savedCol;

    return attacked;
}