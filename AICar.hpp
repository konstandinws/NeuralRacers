#pragma once

#include "car.hpp"
#include <vector>
#include "track.hpp"
#include "neuralNetwork.hpp"

struct Sensor {
    float angleOffset;  
    float maxDistance;  
    float readValue;    
    Vector2 endPoint;     
};
class AICar : public Car {
public:
    std::vector<Sensor> sensors;
    bool isDead = false;
    float score = 0.0f;
    NeuralNetwork nn;

    // Pour evite que les voitures fassent du surplace
    float bestDistanceToFinish = std::numeric_limits<float>::max(); 
    float stagnationCounter = 0.0f;
    float stagnationLimit = 3.0f;

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    void updateAI(const Track& track);
    void drawSensors();
    void initializeSensors(float fov, int numRays, float maxDistance);
};