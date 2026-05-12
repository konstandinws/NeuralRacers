#include "trainConfigState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"
#include "trackSerializer.hpp"

TrainConfigState::TrainConfigState() {
    track = loadTrack("Tracks/trackV3.txt");
    aiCars.emplace_back(AICar(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f));

    camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { 0, 0 }; 
    camera.target = { 0, 0 }; 
}

void TrainConfigState::update(App* app)
{
    // control de la caméra
    if(IsKeyDown(KEY_W)) {
        camera.target.y -= 20; // Déplacer la caméra vers le haut
    }
    if(IsKeyDown(KEY_S)) {
        camera.target.y += 20; // Déplacer la caméra vers le bas
    }
    if(IsKeyDown(KEY_A)) {
        camera.target.x -= 20; // Déplacer la caméra vers la gauche
    }
    if(IsKeyDown(KEY_D)) {
        camera.target.x += 20; // Déplacer la caméra vers la droite
    }

    if (IsKeyPressed(KEY_Q)) {
        camera.zoom *= 1.1f; // Zoomer
    }
    if (IsKeyPressed(KEY_E)) {
        camera.zoom /= 1.1f; // Dézoomer
    }

    // // les rayons des capteurs de l'ia
    // for (AICar& aiCar : aiCars) {
    //     aiCar.updateSensors(track);
    // }

    for(AICar& aiCar : aiCars) {
        aiCar.updateAI(track); // deltaTime fixe pour l'instant
    }
}

void TrainConfigState::render(App* app)
{
    BeginMode2D(camera);
    // lignes de collision pour le debug
    app->renderer.drawTrack(&track);
    app->renderer.drawAICars(aiCars);
    for (AICar& aiCar : aiCars) {
        aiCar.drawSensors();
        aiCar.drawCollision();
    }
    
    EndMode2D();
    
}