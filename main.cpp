#include <vector>
#include <iostream>
#include "raylib.h"
#include <random>
#include <queue>
#include <unordered_map>
#include <set>
#include "car.hpp"

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

struct Track {
    std::vector<Point> points;
    std::vector<Line> lines;
    bool isVisible = false;

    void draw(){
        for(Line line : lines) {
            DrawLine(line.a.x, line.a.y, line.b.x, line.b.y, BLUE);
        }
    }
};

struct trackEditor {
    std::vector<Point> points{};
    std::vector<Line> lines{};
    std::queue<Point> q;
    int closeRadius = 20;

    void handleClick(){
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mousePos = GetMousePosition();
            Point p = {mousePos.x, mousePos.y};

            if(points.size() > 3){ 
                Point first = points.front();
                if (pow(first.x - mousePos.x, 2) <= pow(closeRadius, 2) &&
                    pow(first.y - mousePos.y, 2) <= pow(closeRadius, 2)){
                        p.x = first.x;
                        p.y = first.y;
                        std::cout << "Circuit done" << "\n";
                }
            }
            points.push_back(p);
            q.push(p);
            if(q.size() > 1){
                lines.push_back(Line{q.front(), q.back()});
                q.pop();
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            std::cout << "Points : ";
            for(Point p : points) {
                std::cout << "(" << p.x << ", " << p.y << ") ";
            }
            std::cout << "\n";

            std::cout << "Lines : ";
            for(Line l : lines) {
                std::cout << "(" << l.a.x << ", " << l.a.y << ")" << ", " << "(" << l.b.x << ", " << l.b.y << ") ";
            }
            std::cout << "\n";
        }

        if (IsKeyPressed(KEY_SPACE)){
            points.clear();
            lines.clear();
            q.pop();
        }
    }

    Track getTrack() {
        return Track{points, lines};
    }

    void draw(){
        for(Point p : points) {
            DrawCircle(p.x, p.y, 5, YELLOW);    
        }

        for(Line l : lines){
            DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, GREEN);
        }
    }
};


trackEditor t = trackEditor();
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