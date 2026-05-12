#include "trainConfigState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"
#include "trackSerializer.hpp"
#include <iostream>

TrainConfigState::TrainConfigState()
{
    track = loadTrack("Tracks/trackV4.txt");

    createAICars(20);

    camera = {0};
    camera.zoom = 1.0f;
    camera.offset = {0, 0};
    camera.target = {0, 0};
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

    bool allDead = true;

    for(AICar& aiCar : aiCars) {
        aiCar.updateAI(track);
        if (track.checkWallCollision(aiCar)) {
            aiCar.isDead = true;
        }
        if (!aiCar.isDead) {
            allDead = false;
        }
    }

    if (allDead) {
        nextGeneration();
    }
}

// LES FPS ILS ONT APPRIS A FAIRE DU SURPLACE

void TrainConfigState::render(App* app)
{
    BeginMode2D(camera);
    // lignes de collision pour le debug
    app->renderer.drawTrack(&track);
    app->renderer.drawAICars(aiCars);
    for (AICar& aiCar : aiCars) {
        if(!aiCar.isDead) {
            aiCar.drawSensors();
        }
    }
    
    EndMode2D();
    
}

void TrainConfigState::createAICars(int numCars)
{
    for (int i = 0; i < numCars; ++i)
    {
        aiCars.emplace_back(AICar(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f));
    }
}

void TrainConfigState::nextGeneration() {
    // meilleure voiture de la génération précédente
    AICar bestCar = aiCars[0];
    for (const AICar& car : aiCars) {
        if (car.score > bestCar.score) {
            bestCar = car;
        }
    }

    // 2. Préparer le nouveau point de départ (selon ton circuit)
    float startX = track.start.x;
    float startY = track.start.y;
    float startAngle = (track.angleStart * RAD2DEG) + 90.0f;

    // 3. Créer la nouvelle population
    std::vector<AICar> newPopulation;
    
    for (int i = 0; i < aiCars.size(); i++) {
        // On crée une nouvelle voiture toute neuve sur la ligne de départ
        AICar clone(startX, startY, startAngle);
        
        // On lui greffe le cerveau du champion !
        clone.nn = bestCar.nn; 

        // Si ce n'est PAS la première voiture, on la mute 
        // (10% de chances de muter chaque neurone)
        if (i > 0) {
            clone.nn.mutate(0.1f); 
        }

        newPopulation.push_back(clone);
    }

    // 4. On remplace l'ancienne population par la nouvelle
    aiCars = newPopulation;
    
    std::cout << "Nouvelle génération créée avec un score de champion de : " << bestCar.score << std::endl;
}