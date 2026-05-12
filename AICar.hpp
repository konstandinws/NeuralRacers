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

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    void drawSensors();
    void initializeSensors(float fov, int numRays, float maxDistance);
};