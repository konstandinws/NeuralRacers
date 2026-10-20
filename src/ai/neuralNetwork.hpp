#pragma once
#include <vector>
#include <cmath>

struct Layer {
    int numNeurons;
    int numInputsPerNeuron;
    
    std::vector<float> neurons;                     // value of each neuron
    std::vector<float> biases;                      // bias of each neuron
    std::vector<std::vector<float>> weights;        // connection weights

    Layer(int numInputs, int numNeurons);
    void feedForward(const std::vector<float>& inputs);
};

class NeuralNetwork {
public:
    std::vector<Layer> layers;

    NeuralNetwork(const std::vector<int>& topology);

    std::vector<float> feedForward(const std::vector<float>& inputs);

    // neurons per layer, input included (e.g. {10, 8, 2})
    std::vector<int> getTopology() const;

    // adds inputs with zero weights: the network gives exactly the same outputs as before,
    // and training can then learn to use the new inputs
    // returns false if newInputCount is smaller than the current input count
    bool widenInputs(int newInputCount);
    
    // mutationRate: probability of mutating each weight/bias
    // mutationStrength: standard deviation of the added Gaussian noise
    void mutate(float mutationRate, float mutationStrength);

    // child of two parents with the same topology: each neuron (bias + incoming weights) comes from one or the other
    static NeuralNetwork crossover(const NeuralNetwork& parentA, const NeuralNetwork& parentB);
};