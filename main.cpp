#include <vector>
#include <iostream>
#include "raylib.h"
#include <random>
#include <queue>

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

struct Car{
    int width = 20;
    int height = 50; 
    float angle = 0;
    Vector2 pos{W2, H2};
    float rad = angle * DEG2RAD;
    Vector2 velocity = {0.0f, 0.0f};
    float friction = 0.1f;

    Rectangle carRect = { pos.x, pos.y, width, height };
    Vector2 origin = { width / 2, height / 2 };

    void draw(){
        DrawRectanglePro(carRect, origin, angle, RED);
    }

    void handleInput(){
    
        if(IsKeyDown(KEY_LEFT)){
            angle -= 4;
            rad = angle * DEG2RAD;
        }
        if(IsKeyDown(KEY_RIGHT)){
            angle += 4;
            rad = angle * DEG2RAD;
        }
        if(IsKeyDown(KEY_UP)){
            velocity.x += 1 * sin(rad);
            velocity.y -= 1 * cos(rad); 
        }
        if(IsKeyDown(KEY_DOWN)){
            velocity.x -= 1 * sin(rad);
            velocity.y += 1 * cos(rad); 
        }
        if (IsKeyDown(KEY_SPACE))
        {
            std::cout << 10 * cos(rad) << " - " << 10 * sin(rad) << "\n";
        }

        velocity.x *= 1 - friction;
        velocity.y *= 1 - friction;

        carRect.x += velocity.x;
        carRect.y += velocity.y;

    }
};


Car car = Car();

struct Point {
    float x;
    float y;
};

struct Line {
    Point a;
    Point b;
};


struct trackEditor {
    std::vector<Point> points{};
    std::vector<Line> lines{};
    std::queue<Point> q;

    void handleClick(){
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 mousePos = GetMousePosition();
            Point p = {mousePos.x, mousePos.y};
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

int main() {

    InitWindow(W, H, "trackmania 2D");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        car.draw();
        car.handleInput();
        t.handleClick();
        t.draw();
        DrawCircle(W2, H2, 5, WHITE);
        ClearBackground(BLACK);
        EndDrawing();

    }

    CloseWindow();

    return 0;
}