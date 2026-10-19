#pragma once
#include "core/state.hpp"
#include "ai/aiCar.hpp"
#include <vector>
#include <string>

// entrainement en cours sur un circuit, les parametres sont choisis dans TrainConfigState
class TrainingState : public State
{
public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
    void createAICars(int numCars);
    void nextGeneration();
    TrainingState(const std::string& trackPath, int numCars);

private:
    std::vector<AICar> aiCars;
    Track track;
    Camera2D camera;

    // parametres de l'algorithme genetique
    float ELITE_RATIO = 0.1f;        // part des meilleures voitures recopiees telles quelles
    float PARENT_RATIO = 0.2f;       // part des meilleures voitures qui peuvent se reproduire
    float MUTATION_RATE = 0.1f;      // probabilite de muter chaque poids
    float MUTATION_STRENGTH = 0.3f;  // ecart-type du bruit gaussien

    int generation = 1;
    float bestScoreLastGeneration = 0.0f;
};