#include "trainConfigState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"
#include "trackSerializer.hpp"
#include <iostream>

TrainConfigState::TrainConfigState()
{
    track = loadTrack("Tracks/trackTest.txt");

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
        if (track.checkFinishLine(aiCar)) {
            aiCar.score += 1000.0f; // Bonus pour avoir franchi la ligne d'arrivée
            aiCar.isDead = true; // On considère que la voiture a terminé sa course
        }
        if (!aiCar.isDead) {
            allDead = false;
        }
    }

    if (allDead) {
        nextGeneration();
    }
}

// LES FDP ILS ONT APPRIS A FAIRE DU SURPLACE

void TrainConfigState::render(App* app)
{
    BeginMode2D(camera);
    // lignes de collision pour le debug
    app->renderer.drawTrack(&track);
    app->renderer.drawAICars(aiCars);
    
    // fonction de debug pour visualiser les capteurs
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

    // Point de depart
    float startX = track.start.x;
    float startY = track.start.y;
    float startAngle = (track.angleStart * RAD2DEG) + 90.0f;
    
    std::vector<AICar> newPopulation;
    
    for (int i = 0; i < aiCars.size(); i++) {
        // On crée une nouvelle voiture toute neuve sur la ligne de départ
        AICar clone(startX, startY, startAngle);
        
        // on clone la meilleure voiture de la génération précédente
        clone.nn = bestCar.nn; 

        // 100% de chances de muter chaque neurone sauf permier
        if (i > 0) {
            clone.nn.mutate(1.0f); 
        }

        newPopulation.push_back(clone);
    }

    // remplace l'ancienne population par la nouvelle
    aiCars = newPopulation;
    
    std::cout << "Nouvelle génération créée avec un score de champion de : " << bestCar.score << std::endl;
}