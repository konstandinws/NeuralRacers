#pragma once
#include "core/state.hpp"
#include "ai/aiCar.hpp"
#include "io/modelSerializer.hpp"
#include <optional>
#include <vector>
#include <string>

// settings chosen in TrainConfigState
struct TrainingConfig {
    Track track;
    int carCount = 50;
    std::string modelName;                   // file where the best network is saved
    std::optional<SavedModel> startModel;    // empty = new model with random weights
};

// training in progress on a track
// after each generation, the best network so far is saved to assets/models/<modelName>.txt
class TrainingState : public State
{
public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
    void nextGeneration();
    explicit TrainingState(TrainingConfig config);

private:
    // startNetwork: starting network (loaded model), otherwise random weights
    void createAICars(int numCars, const NeuralNetwork* startNetwork);

    std::vector<AICar> aiCars;
    Track track;
    Camera2D camera;

    std::string modelName;
    std::optional<NeuralNetwork> bestNetwork;  // best network since training started
    float bestScoreEver = 0.0f;
    bool lastSaveFailed = false;

    // genetic algorithm settings
    float ELITE_RATIO = 0.1f;        // share of the best cars copied as is
    float PARENT_RATIO = 0.2f;       // share of the best cars allowed to reproduce
    float MUTATION_RATE = 0.1f;      // probability of mutating each weight
    float MUTATION_STRENGTH = 0.3f;  // standard deviation of the Gaussian noise

    int generation = 1;
    float bestScoreLastGeneration = 0.0f;
};