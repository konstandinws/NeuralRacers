#include "renderer.hpp"
#include "raymath.h"

// utiliser des references pour etre plus moderne, mais j'ai commencé avec des pointeurs 

void Renderer::drawCar(Car* car) {
    Rectangle renderRect = { car->pos.x, car->pos.y, (float)car->width, (float)car->height };
    
    Vector2 renderOrigin = { (float)car->width / 2.0f, (float)car->height / 2.0f };

    DrawRectanglePro(renderRect, renderOrigin, car->angle, RED);
}

void Renderer::showHUD(const Car& car, float raceTime, float endRaceTime) {
    DrawRectangle(0, 800 - 80, 1200, 80, Fade(LIGHTGRAY, 0.7f));

    // calcule l'hypothenuse
    float speedPixels = Vector2Length(car.velocity);

    int displaySpeed = (int)(speedPixels * 15.0f);

    const char* textVitesse = TextFormat("VITESSE : %03d KM/H", displaySpeed); 
    const char* textTemps = TextFormat("TEMPS : %.2f s", raceTime);
    const char* textMeilleurTemps = TextFormat("MEILLEUR : %.2f s", endRaceTime);
    // On dessine le texte par-dessus le rectangle
    DrawText(textVitesse, 50, 800 - 55, 30, GREEN);
    DrawText(textTemps, 1200 - 350, 800 - 55, 30, YELLOW);
    if(endRaceTime < std::numeric_limits<float>::max()) {
        DrawText(textMeilleurTemps, 1200 - 350, 800 - 25, 30, GOLD);
    }
}

void Renderer::showCarBindings() {
    DrawText("Fleches: Conduire | R: Reset | ECHAP: Pause", 10, 10, 20, WHITE);
}

void Renderer::drawTrack(Track* track) {
    for(Line& l : track->lines){
        DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, WHITE);
    }
    DrawLine(track->finish.a.x, track->finish.a.y, track->finish.b.x, track->finish.b.y, YELLOW);
}

void Renderer::drawAICars(const std::vector<AICar>& aiCars) {
    Rectangle renderRect;
    Vector2 renderOrigin;

    for (const AICar& aiCar : aiCars) {
        if (!aiCar.isDead) {
            renderRect.x = aiCar.pos.x;
            renderRect.y = aiCar.pos.y;
            renderOrigin.x = (float)aiCar.width / 2.0f;
            renderOrigin.y = (float)aiCar.height / 2.0f;
            if(aiCar.isDead){
                DrawRectanglePro(renderRect, renderOrigin, aiCar.angle, Color{ 230, 41, 55, 100});
            } else {
                DrawRectanglePro(renderRect, renderOrigin, aiCar.angle, RED);
            }
        }
    }

}
