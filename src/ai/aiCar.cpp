#include "ai/aiCar.hpp"
#include "raymath.h"
#include <iostream>

AICar::AICar(float x, float y, float angle) : Car(x, y, angle), nn(AI_TOPOLOGY) {
    initializeSensors(90.0f, SENSOR_COUNT, 500.0f);
}

void AICar::initializeSensors(float fov, int numRays, float maxDistance) {
    // evite de mettre le nombre de rayons a 1 pour eviter la division par zero
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

        // collision avec les murs
        float closestDist = sensor.maxDistance;
        Vector2 closestPoint = { sensorEnd.x, sensorEnd.y };

        // pour optimiser il faudrait ajouter une grille
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

        sensor.readValue = closestDist / sensor.maxDistance; // Normalise a [0, 1]
        sensor.endPoint = closestPoint;
    }
}

std::vector<float> AICar::computeInputs() const {
    std::vector<float> inputs;
    for (const Sensor& s : sensors) {
        inputs.push_back(s.readValue); // On envoie les valeurs (0.0 à 1.0)
    }

    // vitesse dans le repere de la voiture, pour qu'elle sache quand freiner et si elle derape
    // vitesse max atteinte pedale a fond : v = (v + a) * (1 - friction)  =>  v = a * (1 - friction) / friction
    float maxSpeed = MAX_ACCELERATION * (1.0f - friction) / friction;
    Vector2 forward = { sinf(rad), -cosf(rad) };  // meme direction que Car::accelerate
    Vector2 right = { cosf(rad), sinf(rad) };
    inputs.push_back(Clamp(Vector2DotProduct(velocity, forward) / maxSpeed, -1.0f, 1.0f)); // negative en marche arriere
    inputs.push_back(Clamp(Vector2DotProduct(velocity, right) / maxSpeed, -1.0f, 1.0f));   // derapage gauche / droite

    return inputs;
}

void AICar::updateAI(const Track& track) {
    if (isDead) return;

    updateSensors(track);
    aliveTimer += GetFrameTime();

    std::vector<float> outputs = nn.feedForward(computeInputs());

    //outputs sont normalisés entre -1 et 1 grâce à la fonction d'activation tanh

    float turnAmount = outputs[0] * MAX_TURN;
    float accAmount = outputs[1] * MAX_ACCELERATION;

    this->turn(turnAmount);
    this->accelerate(accAmount);

    Car::update();

    updateProgress(track);

    // le score est recalcule a chaque frame a partir de la progression, il ne s'accumule pas
    score = progress * GATE_SCORE - aliveTimer * TIME_PENALTY;

    if (hasFinished) {
        isDead = true; // On considère que la voiture a terminé sa course
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
            lowSpeedTimer += GetFrameTime(); // Le chronomètre tourne
        } else {
            lowSpeedTimer = 0.0f;       // Elle a réaccéléré, on pardonne et on remet à zéro.
        }

        // Sentence irrévocable
        if (lowSpeedTimer > MAX_SLOW_TIME) {
            isDead = true;
            score -= 20.0f; // On pénalise les conducteurs lents
        }
    }
}

void AICar::updateProgress(const Track& track) {
    int gateCount = track.getGateCount();

    // on ne regarde que la prochaine porte : impossible de valider un checkpoint dans le desordre
    if (track.checkLineCrossed(*this, track.getGate(nextGate))) {
        nextGate++;
        if (nextGate == gateCount) {
            hasFinished = true;
            progress = (float)gateCount;
            return;
        }
    }

    // avancement entre la porte precedente (ou le depart) et la prochaine
    Vector2 previous = (nextGate == 0) ? Vector2{track.start.x, track.start.y} : track.getGateCenter(nextGate - 1);
    Vector2 target = track.getGateCenter(nextGate);

    float segmentLength = Vector2Distance(previous, target);
    float remaining = Vector2Distance(pos, target);
    float fraction = (segmentLength > 0.0f) ? Clamp(1.0f - remaining / segmentLength, 0.0f, 1.0f) : 0.0f;

    progress = nextGate + fraction;
}