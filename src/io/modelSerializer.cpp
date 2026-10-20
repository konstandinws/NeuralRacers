#include "io/modelSerializer.hpp"
#include "io/fileLoader.hpp"
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>

static const std::string HEADER = "NEURAL_NETWORK";
static const int FORMAT_VERSION = 1;

static std::string modelPath(const std::string& name) {
    return MODELS_DIR + name + ".txt";
}

bool saveModel(const std::string& name, const NeuralNetwork& network, int generation) {
    std::error_code error;
    std::filesystem::create_directories(MODELS_DIR, error);

    // write to a temporary file then rename it:
    // if the game is closed while writing, the old model stays intact
    std::string path = modelPath(name);
    std::string tempPath = path + ".tmp";
    {
        std::ofstream file(tempPath);
        if (!file.is_open()) {
            return false;
        }

        // full precision to get exactly the same weights back when loading
        file.precision(std::numeric_limits<float>::max_digits10);

        file << HEADER << " " << FORMAT_VERSION << "\n";
        file << "GENERATION " << generation << "\n";
        file << "TOPOLOGY";
        for (int size : network.getTopology()) {
            file << " " << size;
        }
        file << "\n";

        for (const Layer& layer : network.layers) {
            for (int i = 0; i < layer.numNeurons; i++) {
                file << layer.biases[i];
                for (float weight : layer.weights[i]) {
                    file << " " << weight;
                }
                file << "\n";
            }
        }

        if (!file.good()) {
            return false;
        }
    }

    std::filesystem::rename(tempPath, path, error);
    return !error;
}

std::optional<SavedModel> loadModel(const std::string& name) {
    std::ifstream file(modelPath(name));
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string word;
    int version = 0;
    if (!(file >> word >> version) || word != HEADER || version != FORMAT_VERSION) {
        return std::nullopt;
    }

    int generation = 0;
    if (!(file >> word >> generation) || word != "GENERATION") {
        return std::nullopt;
    }

    // the TOPOLOGY line holds a variable number of sizes
    std::string topologyLine;
    std::getline(file >> std::ws, topologyLine);
    std::istringstream topologyStream(topologyLine);
    std::vector<int> topology;
    if (!(topologyStream >> word) || word != "TOPOLOGY") {
        return std::nullopt;
    }
    for (int size; topologyStream >> size;) {
        if (size <= 0) {
            return std::nullopt;
        }
        topology.push_back(size);
    }
    if (topology.size() < 2) {
        return std::nullopt;
    }

    NeuralNetwork network(topology);
    for (Layer& layer : network.layers) {
        for (int i = 0; i < layer.numNeurons; i++) {
            if (!(file >> layer.biases[i])) {
                return std::nullopt;
            }
            for (float& weight : layer.weights[i]) {
                if (!(file >> weight)) {
                    return std::nullopt;
                }
            }
        }
    }

    return SavedModel{network, generation};
}

std::vector<std::string> listModels() {
    std::vector<std::string> files;
    loadFilesNameFromFolder(MODELS_DIR, files);

    std::vector<std::string> names;
    for (const std::string& file : files) {
        names.push_back(std::filesystem::path(file).stem().string());
    }
    return names;
}

bool modelExists(const std::string& name) {
    return std::filesystem::exists(modelPath(name));
}

std::string nextDefaultModelName() {
    return nextFreeName(MODELS_DIR, "new_model_");
}
