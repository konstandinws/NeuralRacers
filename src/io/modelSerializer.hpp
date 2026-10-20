#pragma once
#include <optional>
#include <string>
#include <vector>
#include "ai/neuralNetwork.hpp"

// models folder (trained networks), relative to the folder the game is launched from
inline const std::string MODELS_DIR = "assets/models/";

struct SavedModel {
    NeuralNetwork network;
    int generation = 0;  // number of generations already trained
};

// text format (assets/models/<name>.txt):
//   NEURAL_NETWORK 1
//   GENERATION 42
//   TOPOLOGY 10 8 2
//   then one line per neuron, layer by layer: bias followed by incoming weights
bool saveModel(const std::string& name, const NeuralNetwork& network, int generation);

// nullopt if the file does not exist or is invalid
std::optional<SavedModel> loadModel(const std::string& name);

// model names (without extension), sorted
std::vector<std::string> listModels();

bool modelExists(const std::string& name);

// first free name of the form new_model_X
std::string nextDefaultModelName();
