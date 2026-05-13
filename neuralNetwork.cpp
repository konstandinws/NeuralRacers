#include "neuralNetwork.hpp"
#include <algorithm> 

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
        biases[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; 
        
        for (int j = 0; j < numInputs; j++) {
            weights[i][j] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
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

void NeuralNetwork::mutate(float mutationRate) {
    for (Layer& layer : layers) {
        for (int i = 0; i < layer.numNeurons; i++) {
            
            // Muter le biais -0.5 à 0.5
            if (((float)rand() / RAND_MAX) < mutationRate) {
                layer.biases[i] += (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * 0.5f; 

                layer.biases[i] = std::clamp(layer.biases[i], -2.0f, 2.0f); // clamp les biais pour eviter les valeurs extrêmes
            } 

            // 2. Muter les poids
            for (int j = 0; j < layer.numInputsPerNeuron; j++) {
                if (((float)rand() / RAND_MAX) < mutationRate) {
                    layer.weights[i][j] += (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * 0.5f;
                    layer.weights[i][j] = std::clamp(layer.weights[i][j], -2.0f, 2.0f); // clamp les poids pour eviter les valeurs extrêmes entre -2 et 2
                }
            }
        }
    }
}