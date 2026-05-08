#include "playState.hpp"
#include "trackSerializer.hpp"
#include "raymath.h"


PlayState::PlayState(const std::string& trackName) : car(600, 400), raceTime(0.0f) {
    // Charger le track à partir du nom fourni
    track = loadTrack(trackName);

    camera = { 0 };
    camera.target = car.pos;
    camera.offset = { 600, 400 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

}

void PlayState::update(App* app) {
    car.handleInput();
    car.update();
    camera.target = car.pos;
    raceTime += GetFrameTime();

    if(IsKeyPressed(KEY_R)) {
        car.reset(600, 400);
        raceTime = 0.0f;
    }
}

void PlayState::render(App* app) {

    BeginMode2D(camera);
    track.draw();
    car.drawCollision();
    DrawCircle(600, 400, 5, RED);
    EndMode2D();

    DrawRectangle(0, 800 - 80, 1200, 80, Fade(BLACK, 0.7f));

    // calcule l'hypothenuse
    float speedPixels = Vector2Length(car.velocity);

    int displaySpeed = (int)(speedPixels * 15.0f);

    const char* textVitesse = TextFormat("VITESSE : %03d KM/H", displaySpeed); 
    const char* textTemps = TextFormat("TEMPS : %.2f s", raceTime);

    // On dessine le texte par-dessus le rectangle
    DrawText(textVitesse, 50, 800 - 55, 30, GREEN);
    DrawText(textTemps, 1200 - 350, 800 - 55, 30, YELLOW);
}