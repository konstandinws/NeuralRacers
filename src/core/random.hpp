#pragma once
#include <random>

// generator shared by the whole project, seeded once with a different seed on each launch
// (rand() without srand always gave the same population)
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
