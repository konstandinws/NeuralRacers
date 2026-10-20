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
        generation = config.startModel->generation + 1;  // continue the model's counter
    }

    // at least 2 cars are needed to have 2 parents in nextGeneration
    createAICars(std::max(2, config.carCount), startNetwork);

    camera = {};
    camera.zoom = 1.0f;
    camera.offset = {0, 0};
    camera.target = {0, 0};
}

void TrainingState::update(App* app)
{
    (void)app;
    // camera controls
    if(IsKeyDown(KEY_W)) {
        camera.target.y -= 20; // move the camera up
    }
    if(IsKeyDown(KEY_S)) {
        camera.target.y += 20; // move the camera down
    }
    if(IsKeyDown(KEY_A)) {
        camera.target.x -= 20; // move the camera left
    }
    if(IsKeyDown(KEY_D)) {
        camera.target.x += 20; // move the camera right
    }

    if (IsKeyPressed(KEY_Q)) {
        camera.zoom *= 1.1f; // zoom in
    }
    if (IsKeyPressed(KEY_E)) {
        camera.zoom /= 1.1f; // zoom out
    }

    bool allDead = true;

    for(AICar& aiCar : aiCars) {
        aiCar.updateAI(track);
        // the finish is handled by AICar::updateProgress (last gate)
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

// at some point they learned to just stand still

void TrainingState::render(App* app)
{
    BeginMode2D(camera);
    // collision lines for debugging
    app->renderer.drawTrack(&track);
    app->renderer.drawAICars(aiCars);
    
    // debug function to visualize the sensors
    for (AICar& aiCar : aiCars) {
        if(!aiCar.isDead) {
            aiCar.drawSensors();
        }
    }
    
    EndMode2D();

    DrawText(TextFormat("Model: %s | Generation %d | Previous best score: %.0f", modelName.c_str(), generation, bestScoreLastGeneration), 10, 10, 20, RAYWHITE);
    if (lastSaveFailed) {
        DrawText("Failed to save the model", 10, 35, 20, RED);
    }
}

void TrainingState::createAICars(int numCars, const NeuralNetwork* startNetwork)
{
    for (int i = 0; i < numCars; ++i)
    {
        AICar car(track.start.x, track.start.y, (track.angleStart * RAD2DEG) + 90.0f);

        // loaded model: the 1st car keeps it as is, the others are variants of it
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
    // ranking of the previous generation, best first
    std::sort(aiCars.begin(), aiCars.end(), [](const AICar& a, const AICar& b) {
        return a.score > b.score;
    });

    int populationSize = (int)aiCars.size();
    int eliteCount = std::max(1, (int)(populationSize * ELITE_RATIO));
    int parentCount = std::max(2, (int)(populationSize * PARENT_RATIO));

    // starting point
    float startX = track.start.x;
    float startY = track.start.y;
    float startAngle = (track.angleStart * RAD2DEG) + 90.0f;

    std::vector<AICar> newPopulation;

    for (int i = 0; i < populationSize; i++) {
        // create a brand new car on the starting line
        AICar child(startX, startY, startAngle);

        if (i < eliteCount) {
            // the best ones are kept unchanged so the best solution is never lost
            child.nn = aiCars[i].nn;
        } else {
            // the others are children of two parents picked among the best, then slightly mutated
            const AICar& parentA = aiCars[randomInt(0, parentCount - 1)];
            const AICar& parentB = aiCars[randomInt(0, parentCount - 1)];
            child.nn = NeuralNetwork::crossover(parentA.nn, parentB.nn);
            child.nn.mutate(MUTATION_RATE, MUTATION_STRENGTH);
        }

        newPopulation.push_back(child);
    }

    bestScoreLastGeneration = aiCars[0].score;
    std::cout << "Generation " << generation << ": best score " << aiCars[0].score << std::endl;

    // save: keep the best network seen so far, so the model is never replaced by a worse one
    if (!bestNetwork || aiCars[0].score > bestScoreEver) {
        bestNetwork = aiCars[0].nn;
        bestScoreEver = aiCars[0].score;
    }
    lastSaveFailed = !saveModel(modelName, *bestNetwork, generation);

    generation++;

    // replace the old population with the new one
    aiCars = newPopulation;
}