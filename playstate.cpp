#include "playState.hpp"
#include "trackSerializer.hpp"
#include "raymath.h"
#include "app.hpp"
#include "track.hpp"
#include <iostream>


PlayState::PlayState(const std::string& trackName) : raceTime(0.0f) {
    // Charger le track à partir du nom fourni
    track = loadTrack(trackName);

    // FIX abominable je me suis perdu dans les randiants vs degres
    car = Car(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f); 

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

    if(car.pos != Vector2{track.start.x, track.start.y}) {
        raceTime += GetFrameTime();
    }

    if (track.checkWallCollision(car)) {
        car.reset(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f);
        raceTime = 0.0f;
    }

    if (track.checkFinishLine(car)) {
        if (raceTime < endRaceTime){
            endRaceTime = raceTime;
        }
        car.reset(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f);
        raceTime = 0.0f;
    }

    if(IsKeyPressed(KEY_R)) {
        car.reset(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f);
        raceTime = 0.0f;
    }
}

void PlayState::render(App* app) {

    BeginMode2D(camera);
    // lignes de collision pour le debug
    car.drawCollision();
    app->renderer.drawTrack(&track);
    app->renderer.drawCar(&car);
   
    EndMode2D();
    app->renderer.showHUD(car, raceTime, endRaceTime);
    app->renderer.showCarBindings();
}