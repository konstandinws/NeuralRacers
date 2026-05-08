#include "renderer.hpp"
#include "raymath.h"

// utiliser des references pour etre plus moderne, mais j'ai commencé avec des pointeurs 

void Renderer::drawCar(Car* car) {
    Rectangle renderRect = { car->pos.x, car->pos.y, (float)car->width, (float)car->height };
    
    Vector2 renderOrigin = { (float)car->width / 2.0f, (float)car->height / 2.0f };

    DrawRectanglePro(renderRect, renderOrigin, car->angle, RED);
}

void Renderer::showCarInfos(Car* car) {
    DrawText(TextFormat("Position: (%.2f, %.2f)", car->pos.x, car->pos.y), 10, 10, 20, WHITE);
    DrawText(TextFormat("Angle: %.2f", car->angle), 10, 40, 20, WHITE);
    DrawText(TextFormat("Velocity: (%.2f, %.2f)", car->velocity.x, car->velocity.y), 10, 70, 20, WHITE);
}

void Renderer::showHUD(const Car& car, float raceTime) {
        // encapsuler ca dans une fonction du renderer
    DrawRectangle(0, 800 - 80, 1200, 80, Fade(LIGHTGRAY, 0.7f));

    // calcule l'hypothenuse
    float speedPixels = Vector2Length(car.velocity);

    int displaySpeed = (int)(speedPixels * 15.0f);

    const char* textVitesse = TextFormat("VITESSE : %03d KM/H", displaySpeed); 
    const char* textTemps = TextFormat("TEMPS : %.2f s", raceTime);

    // On dessine le texte par-dessus le rectangle
    DrawText(textVitesse, 50, 800 - 55, 30, GREEN);
    DrawText(textTemps, 1200 - 350, 800 - 55, 30, YELLOW);
}
void Renderer::showCarBindings() {
    DrawText("Fleches: Conduire | R: Reset | ECHAP: Pause", 10, 10, 20, WHITE);
}
