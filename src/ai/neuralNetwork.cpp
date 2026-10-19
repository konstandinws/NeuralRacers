#include "ai/neuralNetwork.hpp"
#include <algorithm>
#include "core/random.hpp"

// init d'une chouche
Layer::Layer(int numInputs, int numNeurons) {
    this->numNeurons = numNeurons;
    this->numInputsPerNeuron = numInputs;
    
    neurons.resize(numNeurons, 0.0f);
    biases.resize(numNeurons, 0.0f);
    weights.resize(numNeurons, std::vector<float>(numInputs, 0.0f));

    // on met des valeurs au hasard pour les poids et les biais 
    for (int i = 0; i < numNeurons; i++) {
        // Un nombre au hasard entre -1 et 1
        biases[i] = randomFloat(-1.0f, 1.0f);

        for (int j = 0; j < numInputs; j++) {
            weights[i][j] = randomFloat(-1.0f, 1.0f);
        }
    }
}

// feed forward d'une couche
void Layer::feedForward(const std::vector<float>& inputs) {
    for (int i = 0; i < numNeurons; i++) {
        float sum = biases[i];
        for (int j = 0; j < numInputsPerNeuron; j++) {
            sum += inputs[j] * weights[i][j];
        }
        // Fonction d'activation tanh
        neurons[i] = std::tanh(sum); 
    }
}


NeuralNetwork::NeuralNetwork(const std::vector<int>& topology) {
    // Si topology = {5, 8, 2}, on crée 2 couches de connexion :
    // Couche 1 : 5 entrées -> 8 neurones (entree)
    // Couche 2 : 8 entrées -> 2 neurones (hidden)
    // Couche 3 : 2 neurones (sortie)
    for (size_t i = 1; i < topology.size(); i++) {
        layers.emplace_back(topology[i - 1], topology[i]);
    }
}

std::vector<float> NeuralNetwork::feedForward(const std::vector<float>& inputs) {
    std::vector<float> currentInputs = inputs;

    // L'information traverse les couches une par une
    for (Layer& layer : layers) {
        layer.feedForward(currentInputs);
        currentInputs = layer.neurons; // La sortie devient l'entrée de la couche suivante
    }

    return currentInputs; // Renvoie les 2 valeurs finales (volant et pédales)
}

std::vector<int> NeuralNetwork::getTopology() const {
    std::vector<int> topology;
    if (!layers.empty()) {
        topology.push_back(layers[0].numInputsPerNeuron);
    }
    for (const Layer& layer : layers) {
        topology.push_back(layer.numNeurons);
    }
    return topology;
}

void NeuralNetwork::mutate(float mutationRate, float mutationStrength) {
    // bruit gaussien : la plupart des mutations sont petites, quelques-unes plus grandes
    std::normal_distribution<float> noise(0.0f, mutationStrength);

    for (Layer& layer : layers) {
        for (int i = 0; i < layer.numNeurons; i++) {

            // 1. Muter le biais
            if (randomFloat(0.0f, 1.0f) < mutationRate) {
                layer.biases[i] += noise(rng());

                layer.biases[i] = std::clamp(layer.biases[i], -2.0f, 2.0f); // clamp les biais pour eviter les valeurs extrêmes
            }

            // 2. Muter les poids
            for (int j = 0; j < layer.numInputsPerNeuron; j++) {
                if (randomFloat(0.0f, 1.0f) < mutationRate) {
                    layer.weights[i][j] += noise(rng());
                    layer.weights[i][j] = std::clamp(layer.weights[i][j], -2.0f, 2.0f); // clamp les poids pour eviter les valeurs extrêmes entre -2 et 2
                }
            }
        }
    }
}

NeuralNetwork NeuralNetwork::crossover(const NeuralNetwork& parentA, const NeuralNetwork& parentB) {
    NeuralNetwork child = parentA;

    // on echange des neurones entiers plutot que des poids isoles :
    // les poids d'un meme neurone fonctionnent ensemble, les melanger casse ce qu'il a appris
    for (size_t l = 0; l < child.layers.size(); l++) {
        Layer& layer = child.layers[l];
        const Layer& other = parentB.layers[l];

        for (int i = 0; i < layer.numNeurons; i++) {
            if (randomInt(0, 1) == 1) {
                layer.biases[i] = other.biases[i];
                layer.weights[i] = other.weights[i];
            }
        }
    }

    return child;
}