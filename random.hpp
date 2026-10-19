#pragma once
#include <random>

// generateur partage par tout le projet, initialise une seule fois avec une graine differente a chaque lancement
// (rand() sans srand donnait toujours la meme population)
inline std::mt19937& rng() {
    static std::mt19937 generator(std::random_device{}());
    return generator;
}

inline float randomFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(rng());
}

inline int randomInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rng());
}
