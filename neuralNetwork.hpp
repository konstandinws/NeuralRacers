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

    std::vector<float> feedForward(const std::vector<float>& inputs);
    
    void mutate(float mutationRate);
};