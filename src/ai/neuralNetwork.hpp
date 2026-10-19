#pragma once
#include <vector>
#include <cmath>

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

    // nombre de neurones par couche, entree comprise (ex : {10, 8, 2})
    std::vector<int> getTopology() const;
    
    // mutationRate : probabilite de muter chaque poids/biais
    // mutationStrength : ecart-type du bruit gaussien ajoute
    void mutate(float mutationRate, float mutationStrength);

    // enfant de deux parents de meme topologie : chaque neurone (biais + poids entrants) vient de l'un ou l'autre
    static NeuralNetwork crossover(const NeuralNetwork& parentA, const NeuralNetwork& parentB);
};