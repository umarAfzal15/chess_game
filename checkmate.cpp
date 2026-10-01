#include "raylib.h"
#include "board.h"

bool Board::checkMate(int color){

    int savedRow = takeRow;
    int savedCol = takeCol;

    bool isCheckMate = false;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            for(int k = 0; k < 8; k++){
                for(int l = 0; l < 8; l++){
                    int piece = grid[i][j];

                    if(piece == 0 || piece/10 == color){
                        continue;
                    }

                    int enemy = piece/10;
                    int pieceType = piece%10;

                    takeRow = i;
                    takeCol = j;

                    if(pieceType == 1){
                        isCheckMate = pawnRule(k, l, enemy);
                    }
                    else if(pieceType == 2){
                        isCheckMate = knightRule(k, l, enemy);
                    }
                    else if(pieceType == 3){
                        isCheckMate = bishopRule(k, l, enemy);
                    }
                    else if(pieceType == 4){
                        isCheckMate = rookRule(k, l, enemy);
                    }
                    else if(pieceType == 5){
                        isCheckMate = queenRule(k, l, enemy);
                    }
                    else if(pieceType == 6){
                        isCheckMate = kingRule(k, l, enemy);
                    }

                    if(isCheckMate){
                        int captured = grid[k][l];

                        grid[k][l] = grid[i][j];
                        grid[i][j] = 0;

                        bool stillAttacked = isCheck(enemy);

                        grid[i][j] = grid[k][l];
                        grid[k][l] = captured;

                        if(stillAttacked){
                            isCheckMate = false;
                        }
                    }

                    if(isCheckMate){
                        break;
                    }
                }
                if(isCheckMate){
                    break;
                }
            }

            if(isCheckMate){
                break;
            }
        }

        if(isCheckMate){
            break;
        }

    }

    takeRow = savedRow;
    takeCol = savedCol;

    return !isCheckMate;
}