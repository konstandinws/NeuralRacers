#include <vector>
#include <iostream>
#include "raylib.h"
#include <random>
#include <queue>
#include <unordered_map>
#include <set>
#include "car.hpp"
#include "track.hpp"
#include "trackEditor.hpp"

const int W = 1200;
const int H = 800;
const int W2 = W/2;
const int H2 = H/2;

const int CELL_SIZE = 200;

void drawGrid() {
    for(int i = CELL_SIZE; i < W; i += CELL_SIZE){
        DrawLine(i, 0, i, H, LIGHTGRAY);
    }
    for(int i = CELL_SIZE; i < H; i += CELL_SIZE){
        DrawLine(0, i, W, i, LIGHTGRAY);
    }
}

Car car = Car(W2, H2);

TrackEditor t = TrackEditor();
Track track;

const int gridSize = 200; 

// il est problable que la unordered_map ne sache pas hasher un Vector2 car structure perso
// soit implementer has_function de unordered_map ou d'en faire une perso pour acceder a tous les elements
// std::unordered_map<Vector2, std::set<Line>> grid;

// parcourir tous les murs et calculer dans quel zone ils se situent
void collision() {
    Vector2 collisionPoint = {0, 0};
    for(Line l : track.lines){
        for(Line carLine : car.carLines){
            if(CheckCollisionLines(Vector2{carLine.a.x, carLine.a.y}, Vector2{carLine.b.x, carLine.b.y}, Vector2{l.a.x, l.a.y}, Vector2{l.b.x, l.b.y}, &collisionPoint)){
                car.carRect.x = W2;
                car.carRect.y = H2;
            }
        }   
    }
}


int main() {

    InitWindow(W, H, "trackmania 2D");
    SetTargetFPS(60);
    

    while(!WindowShouldClose()) {
        BeginDrawing();
        car.draw();
        car.drawCollision();
        car.handleInput();
        t.handleClick();
        t.draw();
        collision();
        DrawCircle(W2, H2, 5, WHITE);

        if(IsKeyPressed(KEY_E)) {
            track = t.getTrack();
        }

        if(IsKeyPressed(KEY_V)) {
            track.isVisible = true;
        }

        if(track.isVisible) {
            track.draw();
        }

        if(IsKeyPressed(KEY_A)){
            std::cout << car.pos.x << ", " << car.pos.y << "\n";
        }

        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}