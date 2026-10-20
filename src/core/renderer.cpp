#include "core/renderer.hpp"
#include "raymath.h"

// references would be more modern, but I started with pointers 

void Renderer::drawCar(Car* car) {
    Rectangle renderRect = { car->pos.x, car->pos.y, (float)car->width, (float)car->height };
    
    Vector2 renderOrigin = { (float)car->width / 2.0f, (float)car->height / 2.0f };

    DrawRectanglePro(renderRect, renderOrigin, car->angle, RED);
}

void Renderer::showHUD(const Car& car, float raceTime, float endRaceTime) {
    // bottom bar, sized from the window instead of hardcoded 1200x800
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawRectangle(0, screenHeight - 80, screenWidth, 80, Fade(LIGHTGRAY, 0.7f));

    // computes the hypotenuse
    float speedPixels = Vector2Length(car.velocity);

    int displaySpeed = (int)(speedPixels * 15.0f);

    const char* textSpeed = TextFormat("SPEED: %03d KM/H", displaySpeed); 
    const char* textTime = TextFormat("TIME: %.2f s", raceTime);
    const char* textBestTime = TextFormat("BEST: %.2f s", endRaceTime);
    // draw the text over the rectangle
    DrawText(textSpeed, 50, screenHeight - 55, 30, GREEN);
    DrawText(textTime, screenWidth - 350, screenHeight - 55, 30, YELLOW);
    if(endRaceTime < std::numeric_limits<float>::max()) {
        DrawText(textBestTime, screenWidth - 350, screenHeight - 25, 30, GOLD);
    }
}

void Renderer::showCarBindings() {
    DrawText("Arrows: Drive | R: Reset | ESC: Pause", 10, 10, 20, WHITE);
}

void Renderer::drawTrack(Track* track) {
    for(Line& l : track->lines){
        DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, WHITE);
    }
    for(Line& c : track->checkpoints){
        DrawLine(c.a.x, c.a.y, c.b.x, c.b.y, Fade(SKYBLUE, 0.3f));
    }
    DrawLine(track->finish.a.x, track->finish.a.y, track->finish.b.x, track->finish.b.y, YELLOW);
}

void Renderer::drawAICars(const std::vector<AICar>& aiCars) {
    for (const AICar& aiCar : aiCars) {
        Rectangle renderRect = { 
            aiCar.pos.x, 
            aiCar.pos.y, 
            (float)aiCar.width, 
            (float)aiCar.height 
        };
        Vector2 renderOrigin = { (float)aiCar.width / 2.0f, (float)aiCar.height / 2.0f };

        if (!aiCar.isDead) {
            DrawRectanglePro(renderRect, renderOrigin, aiCar.angle, RED);
        } else {
            DrawRectanglePro(renderRect, renderOrigin, aiCar.angle, Color{ 230, 41, 55, 100});
        }
    }
}

