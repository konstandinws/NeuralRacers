#include "states/trainingState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "states/pauseState.hpp"
#include "io/trackSerializer.hpp"
#include "core/random.hpp"
#include <iostream>
#include <algorithm>

TrainingState::TrainingState(TrainingConfig config) : modelName(config.modelName)
{
    track = config.track;

    const NeuralNetwork* startNetwork = nullptr;
    if (config.startModel) {
        startNetwork = &config.startModel->network;
        generation = config.startModel->generation + 1;  // on reprend le compteur du modele
    }

    // il faut au moins 2 voitures pour avoir 2 parents dans nextGeneration
    createAICars(std::max(2, config.carCount), startNetwork);

    camera = {};
    camera.zoom = 1.0f;
    camera.offset = {0, 0};
    camera.target = {0, 0};
}

void TrainingState::update(App* app)
{
    (void)app;
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

void TrainingState::render(App* app)
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

    DrawText(TextFormat("Modele : %s | Generation %d | Meilleur score precedent : %.0f", modelName.c_str(), generation, bestScoreLastGeneration), 10, 10, 20, RAYWHITE);
    if (lastSaveFailed) {
        DrawText("Echec de la sauvegarde du modele", 10, 35, 20, RED);
    }
}

void TrainingState::createAICars(int numCars, const NeuralNetwork* startNetwork)
{
    for (int i = 0; i < numCars; ++i)
    {
        AICar car(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f);

        // modele charge : la 1re voiture le garde tel quel, les autres en sont des variantes
        if (startNetwork != nullptr) {
            car.nn = *startNetwork;
            if (i > 0) {
                car.nn.mutate(MUTATION_RATE, MUTATION_STRENGTH);
            }
        }

        aiCars.push_back(car);
    }
}

void TrainingState::nextGeneration() {
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

    // sauvegarde : on garde le meilleur reseau rencontre, pour ne jamais remplacer le modele par un moins bon
    if (!bestNetwork || aiCars[0].score > bestScoreEver) {
        bestNetwork = aiCars[0].nn;
        bestScoreEver = aiCars[0].score;
    }
    lastSaveFailed = !saveModel(modelName, *bestNetwork, generation);

    generation++;

    // remplace l'ancienne population par la nouvelle
    aiCars = newPopulation;
}