#include <iostream>
#include <cstdio>
#include <string>
#include "raylib.h"
#include "board.h"

Board::~Board(){
    for (int i = 0; i < 27; i++) {
        if (textures[i].id != 0) UnloadTexture(textures[i]);
    }
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