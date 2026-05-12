#include "neuralNetwork.hpp"

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