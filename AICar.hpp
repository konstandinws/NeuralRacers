#pragma once

#include "car.hpp"
#include <vector>
#include "track.hpp"

struct Sensor {
    float angleOffset;  
    float maxDistance;  
    float readValue;    
    Point endPoint;     
};
class AICar : public Car {
public:
    std::vector<Sensor> sensors;
    bool isDead = false;
    float score = 0.0f;

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    void drawSensors();
};