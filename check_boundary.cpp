#include "raylib.h"
#include "board.h"

bool Board::isInsideBoard(int x, int y) const {
    return x >= startX && x < startX + 8 * squareSize &&
           y >= startY && y < startY + 8 * squareSize;
}