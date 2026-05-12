#pragma once
#include <vector>
#include <cmath>
#include <cstdlib> 

struct Layer {
    int numNeurons;
    int numInputsPerNeuron;
    
    std::vector<float> neurons;                     // La valeur de chaque neurone
    std::vector<float> biases;                      // Le biais de chaque neurone
    std::vector<std::vector<float>> weights;        // Les poids des connexions 

    Layer(int numInputs, int numNeurons);
    void feedForward(const std::vector<float>& inputs);
};

class NeuralNetwork {
public:
    std::vector<Layer> layers;

    NeuralNetwork(const std::vector<int>& topology);

    // La fonction magique qui convertit les lasers en commandes de conduite !
    std::vector<float> feedForward(const std::vector<float>& inputs);
    
    // (À venir pour l'évolution)
    // void mutate(float mutationRate); 
};