#include "ai/aiCar.hpp"
#include "raymath.h"
#include <iostream>

AICar::AICar(float x, float y, float angle) : Car(x, y, angle), nn(AI_TOPOLOGY) {
    initializeSensors(90.0f, SENSOR_COUNT, 500.0f);
}

void AICar::initializeSensors(float fov, int numRays, float maxDistance) {
    // do not set the ray count to 1, it would divide by zero
    float angleStep = fov / (numRays - 1);
    float startAngle = -fov / 2.0f;

    for (int i = 0; i < numRays; ++i) {
        float angleOffset = startAngle + i * angleStep;
        sensors.push_back({angleOffset, maxDistance, 0.0f, {0.0f, 0.0f}});
    }
}

void AICar::drawSensors() {
    for (const auto& sensor : sensors) {
        DrawLineV(pos, Vector2{sensor.endPoint.x, sensor.endPoint.y}, YELLOW);
    }
}

void AICar::updateSensors(const Track& track) {
    for (auto& sensor : sensors) {
        float sensorAngle = angle - 90.0f + sensor.angleOffset;
        Vector2 sensorDir = { cosf(sensorAngle * DEG2RAD), sinf(sensorAngle * DEG2RAD) };
        Vector2 sensorEnd = { pos.x + sensorDir.x * sensor.maxDistance, pos.y + sensorDir.y * sensor.maxDistance };

        // collision with the walls
        float closestDist = sensor.maxDistance;
        Vector2 closestPoint = { sensorEnd.x, sensorEnd.y };

        // a spatial grid would make this faster
        for (const Line& line : track.lines) {
            Vector2 intersection;
            if (CheckCollisionLines(pos, sensorEnd, Vector2{line.a.x, line.a.y}, Vector2{line.b.x, line.b.y}, &intersection)) {
                float dist = Vector2Distance(pos, Vector2{intersection.x, intersection.y});
                if (dist < closestDist) {
                    closestDist = dist;
                    closestPoint = intersection;
                }
            }
        }

        sensor.readValue = closestDist / sensor.maxDistance; // normalized to [0, 1]
        sensor.endPoint = closestPoint;
    }
}

std::vector<float> AICar::computeInputs() const {
    std::vector<float> inputs;
    for (const Sensor& s : sensors) {
        inputs.push_back(s.readValue); // send the values (0.0 to 1.0)
    }

    // velocity in the car's frame, so it knows when to brake and whether it is sliding
    // top speed with the pedal floored: v = (v + a) * (1 - friction)  =>  v = a * (1 - friction) / friction
    float maxSpeed = MAX_ACCELERATION * (1.0f - friction) / friction;
    Vector2 forward = { sinf(rad), -cosf(rad) };  // same direction as Car::accelerate
    Vector2 right = { cosf(rad), sinf(rad) };
    inputs.push_back(Clamp(Vector2DotProduct(velocity, forward) / maxSpeed, -1.0f, 1.0f)); // negative when reversing
    inputs.push_back(Clamp(Vector2DotProduct(velocity, right) / maxSpeed, -1.0f, 1.0f));   // sliding left / right

    return inputs;
}

void AICar::updateAI(const Track& track) {
    if (isDead) return;

    updateSensors(track);
    aliveTimer += GetFrameTime();

    std::vector<float> outputs = nn.feedForward(computeInputs());

    // outputs are between -1 and 1 thanks to the tanh activation

    float turnAmount = outputs[0] * MAX_TURN;
    float accAmount = outputs[1] * MAX_ACCELERATION;

    this->turn(turnAmount);
    this->accelerate(accAmount);

    Car::update();

    updateProgress(track);

    // the score is recomputed every frame from the progress, it does not accumulate
    score = progress * GATE_SCORE - aliveTimer * TIME_PENALTY;

    if (hasFinished) {
        isDead = true; // the car has finished its race
        return;
    }

    if (progress > bestProgress) {
        bestProgress = progress;
        stagnationCounter = 0.0f;
    } else {
        stagnationCounter += GetFrameTime();
        if (stagnationCounter >= stagnationLimit) {
            isDead = true;
            return;
        }
    }

    if (aliveTimer > GRACE_PERIOD) {
        if (Vector2Length(velocity) < MIN_SPEED) {
            lowSpeedTimer += GetFrameTime(); // the timer is running
        } else {
            lowSpeedTimer = 0.0f;       // it sped up again, reset the timer
        }

        // final verdict
        if (lowSpeedTimer > MAX_SLOW_TIME) {
            isDead = true;
            score -= 20.0f; // penalize slow drivers
        }
    }
}

void AICar::updateProgress(const Track& track) {
    int gateCount = track.getGateCount();

    // only the next gate is checked: checkpoints cannot be passed out of order
    if (track.checkLineCrossed(*this, track.getGate(nextGate))) {
        nextGate++;
        if (nextGate == gateCount) {
            hasFinished = true;
            progress = (float)gateCount;
            return;
        }
    }

    // progress between the previous gate (or the start) and the next one
    Vector2 previous = (nextGate == 0) ? Vector2{track.start.x, track.start.y} : track.getGateCenter(nextGate - 1);
    Vector2 target = track.getGateCenter(nextGate);

    float segmentLength = Vector2Distance(previous, target);
    float remaining = Vector2Distance(pos, target);
    float fraction = (segmentLength > 0.0f) ? Clamp(1.0f - remaining / segmentLength, 0.0f, 1.0f) : 0.0f;

    progress = nextGate + fraction;
}