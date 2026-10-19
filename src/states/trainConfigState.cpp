#include "states/trainConfigState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "states/pauseState.hpp"
#include "io/trackSerializer.hpp"
#include "core/random.hpp"
#include <iostream>
#include <algorithm>

TrainConfigState::TrainConfigState()
{
    track = loadTrack(TRACKS_DIR + "trackTest.txt");

    createAICars(POPULATION_SIZE);

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
        // l'arrivee est geree par AICar::updateProgress (derniere porte)
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

    DrawText(TextFormat("Generation %d | Meilleur score precedent : %.0f", generation, bestScoreLastGeneration), 10, 10, 20, RAYWHITE);
}

void TrainConfigState::createAICars(int numCars)
{
    for (int i = 0; i < numCars; ++i)
    {
        aiCars.emplace_back(AICar(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f));
    }
}

void TrainConfigState::nextGeneration() {
    // classement de la generation precedente, du meilleur au moins bon
    std::sort(aiCars.begin(), aiCars.end(), [](const AICar& a, const AICar& b) {
        return a.score > b.score;
    });

    int populationSize = (int)aiCars.size();
    int eliteCount = std::max(1, (int)(populationSize * ELITE_RATIO));
    int parentCount = std::max(2, (int)(populationSize * PARENT_RATIO));

    // Point de depart
    float startX = track.start.x;
    float startY = track.start.y;
    float startAngle = (track.angleStart * RAD2DEG) + 90.0f;

    std::vector<AICar> newPopulation;

    for (int i = 0; i < populationSize; i++) {
        // On crée une nouvelle voiture toute neuve sur la ligne de départ
        AICar child(startX, startY, startAngle);

        if (i < eliteCount) {
            // les meilleures sont gardees sans modification pour ne jamais perdre la meilleure solution
            child.nn = aiCars[i].nn;
        } else {
            // les autres sont des enfants de deux parents pris parmi les meilleures, puis mutes legerement
            const AICar& parentA = aiCars[randomInt(0, parentCount - 1)];
            const AICar& parentB = aiCars[randomInt(0, parentCount - 1)];
            child.nn = NeuralNetwork::crossover(parentA.nn, parentB.nn);
            child.nn.mutate(MUTATION_RATE, MUTATION_STRENGTH);
        }

        newPopulation.push_back(child);
    }

    bestScoreLastGeneration = aiCars[0].score;
    std::cout << "Generation " << generation << " : meilleur score " << aiCars[0].score << std::endl;
    generation++;

    // remplace l'ancienne population par la nouvelle
    aiCars = newPopulation;
}