#include "ai/neuralNetwork.hpp"
#include <algorithm>
#include "core/random.hpp"

// layer initialization
Layer::Layer(int numInputs, int numNeurons) {
    this->numNeurons = numNeurons;
    this->numInputsPerNeuron = numInputs;
    
    neurons.resize(numNeurons, 0.0f);
    biases.resize(numNeurons, 0.0f);
    weights.resize(numNeurons, std::vector<float>(numInputs, 0.0f));

    // random values for the weights and biases
    for (int i = 0; i < numNeurons; i++) {
        // random number between -1 and 1
        biases[i] = randomFloat(-1.0f, 1.0f);

        for (int j = 0; j < numInputs; j++) {
            weights[i][j] = randomFloat(-1.0f, 1.0f);
        }
    }
}

// feed forward for one layer
void Layer::feedForward(const std::vector<float>& inputs) {
    for (int i = 0; i < numNeurons; i++) {
        float sum = biases[i];
        for (int j = 0; j < numInputsPerNeuron; j++) {
            sum += inputs[j] * weights[i][j];
        }
        // tanh activation function
        neurons[i] = std::tanh(sum); 
    }
}


NeuralNetwork::NeuralNetwork(const std::vector<int>& topology) {
    // if topology = {5, 8, 2}, 2 connection layers are created:
    // layer 1: 5 inputs -> 8 neurons (input)
    // layer 2: 8 inputs -> 2 neurons (hidden)
    // layer 3: 2 neurons (output)
    for (size_t i = 1; i < topology.size(); i++) {
        layers.emplace_back(topology[i - 1], topology[i]);
    }
}

std::vector<float> NeuralNetwork::feedForward(const std::vector<float>& inputs) {
    std::vector<float> currentInputs = inputs;

    // data goes through the layers one by one
    for (Layer& layer : layers) {
        layer.feedForward(currentInputs);
        currentInputs = layer.neurons; // the output becomes the next layer's input
    }

    return currentInputs; // returns the 2 final values (steering and pedals)
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

bool NeuralNetwork::widenInputs(int newInputCount) {
    if (layers.empty() || newInputCount < layers[0].numInputsPerNeuron) {
        return false;
    }
    Layer& first = layers[0];
    for (std::vector<float>& neuronWeights : first.weights) {
        neuronWeights.resize(newInputCount, 0.0f);
    }
    first.numInputsPerNeuron = newInputCount;
    return true;
}

void NeuralNetwork::mutate(float mutationRate, float mutationStrength) {
    // Gaussian noise: most mutations are small, a few are bigger
    std::normal_distribution<float> noise(0.0f, mutationStrength);

    for (Layer& layer : layers) {
        for (int i = 0; i < layer.numNeurons; i++) {

            // 1. mutate the bias
            if (randomFloat(0.0f, 1.0f) < mutationRate) {
                layer.biases[i] += noise(rng());

                layer.biases[i] = std::clamp(layer.biases[i], -2.0f, 2.0f); // clamp the biases to avoid extreme values
            }

            // 2. mutate the weights
            for (int j = 0; j < layer.numInputsPerNeuron; j++) {
                if (randomFloat(0.0f, 1.0f) < mutationRate) {
                    layer.weights[i][j] += noise(rng());
                    layer.weights[i][j] = std::clamp(layer.weights[i][j], -2.0f, 2.0f); // clamp the weights between -2 and 2 to avoid extreme values
                }
            }
        }
    }
}

NeuralNetwork NeuralNetwork::crossover(const NeuralNetwork& parentA, const NeuralNetwork& parentB) {
    NeuralNetwork child = parentA;

    // swap whole neurons rather than single weights:
    // the weights of one neuron work together, mixing them breaks what it learned
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