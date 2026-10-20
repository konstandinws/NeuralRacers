#pragma once

#include "game/car.hpp"
#include <vector>
#include "game/track.hpp"
#include "ai/neuralNetwork.hpp"

inline constexpr int SENSOR_COUNT = 10;

// network inputs: the 10 sensors, then forward speed and lateral speed (sliding)
// 8 hidden neurons, 2 outputs (steering and pedals)
// a saved model can only be reloaded if it has the same topology
// (older 10-input models are adapted on load, see NeuralNetwork::widenInputs)
inline const std::vector<int> AI_TOPOLOGY = {SENSOR_COUNT + 2, 8, 2};

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

    float MAX_TURN = 4.0f;          // degrees per frame at full steering
    float MAX_ACCELERATION = 1.0f;  // pixels per frame added to the speed with the pedal floored

    // progress on the track: index of the next gate to cross (checkpoints, then finish)
    // progress = gates crossed + progress (0 to 1) towards the next one
    int nextGate = 0;
    float progress = 0.0f;
    bool hasFinished = false;

    float GATE_SCORE = 1000.0f;   // points per gate crossed
    float TIME_PENALTY = 10.0f;   // points lost per second, to favor the fastest cars

    // keeps cars from standing still
    float bestProgress = 0.0f;
    float stagnationCounter = 0.0f;
    float stagnationLimit = 3.0f;

    // keeps cars from driving slowly for too long without dying
    float aliveTimer = 0.0f;       // how long the car has been alive
    float lowSpeedTimer = 0.0f;    // how long it has been driving too slowly
    
    float MIN_SPEED = 5.0f;    // minimum speed in pixels per FRAME (top speed is ~9 px/frame)
    float MAX_SLOW_TIME = 5.0f;  // maximum time allowed below MIN_SPEED
    float GRACE_PERIOD = 1.0f;   // time given at the start to speed up

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    // network inputs, in AI_TOPOLOGY order: sensors, forward speed, lateral speed
    std::vector<float> computeInputs() const;
    void updateAI(const Track& track);
    void updateProgress(const Track& track);
    void drawSensors();
    void initializeSensors(float fov, int numRays, float maxDistance);
};