#include "playState.hpp"
#include "trackSerializer.hpp"
#include "raymath.h"
#include "app.hpp"


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
    if (IsKeyDown(KEY_LEFT))  car.turn(-4.0f);
    if (IsKeyDown(KEY_RIGHT)) car.turn(4.0f);
    if (IsKeyDown(KEY_UP))    car.accelerate(1.0f);
    if (IsKeyDown(KEY_DOWN))  car.accelerate(-1.0f);

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
    // lignes de collision pour le debug
    car.drawCollision();
    app->renderer.drawCar(&car);
    app->renderer.drawTrack(&track);
   
    EndMode2D();
    DrawCircle(600, 400, 5, RED);
    app->renderer.showHUD(car, raceTime);
    app->renderer.showCarBindings();


}