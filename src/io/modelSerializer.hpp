#pragma once
#include <optional>
#include <string>
#include <vector>
#include "ai/neuralNetwork.hpp"

// dossier des modeles (reseaux entraines), relatif au dossier depuis lequel le jeu est lance
inline const std::string MODELS_DIR = "assets/models/";

struct SavedModel {
    NeuralNetwork network;
    int generation = 0;  // nombre de generations deja entrainees
};

// format texte (assets/models/<name>.txt) :
//   NEURAL_NETWORK 1
//   GENERATION 42
//   TOPOLOGY 10 8 2
//   puis une ligne par neurone, couche par couche : biais suivi des poids entrants
bool saveModel(const std::string& name, const NeuralNetwork& network, int generation);

// nullopt si le fichier n'existe pas ou est invalide
std::optional<SavedModel> loadModel(const std::string& name);

// noms des modeles (sans extension), tries
std::vector<std::string> listModels();

bool modelExists(const std::string& name);

// premier nom libre de la forme nouveau_modele_X
std::string nextDefaultModelName();
