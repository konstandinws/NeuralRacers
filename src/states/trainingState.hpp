#pragma once
#include "core/state.hpp"
#include "ai/aiCar.hpp"
#include "io/modelSerializer.hpp"
#include <optional>
#include <vector>
#include <string>

// parametres choisis dans TrainConfigState
struct TrainingConfig {
    std::string trackPath;
    int carCount = 50;
    std::string modelName;                   // fichier ou le meilleur reseau est sauvegarde
    std::optional<SavedModel> startModel;    // vide = nouveau modele avec des poids aleatoires
};

// entrainement en cours sur un circuit
// a chaque generation, le meilleur reseau rencontre est sauvegarde dans assets/models/<modelName>.txt
class TrainingState : public State
{
public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
    void nextGeneration();
    explicit TrainingState(TrainingConfig config);

private:
    // startNetwork : reseau de depart (modele charge), sinon poids aleatoires
    void createAICars(int numCars, const NeuralNetwork* startNetwork);

    std::vector<AICar> aiCars;
    Track track;
    Camera2D camera;

    std::string modelName;
    std::optional<NeuralNetwork> bestNetwork;  // meilleur reseau depuis le debut de l'entrainement
    float bestScoreEver = 0.0f;
    bool lastSaveFailed = false;

    // parametres de l'algorithme genetique
    float ELITE_RATIO = 0.1f;        // part des meilleures voitures recopiees telles quelles
    float PARENT_RATIO = 0.2f;       // part des meilleures voitures qui peuvent se reproduire
    float MUTATION_RATE = 0.1f;      // probabilite de muter chaque poids
    float MUTATION_STRENGTH = 0.3f;  // ecart-type du bruit gaussien

    int generation = 1;
    float bestScoreLastGeneration = 0.0f;
};