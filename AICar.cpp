#include "AICar.hpp"
#include "raymath.h"

AICar::AICar(float x, float y, float angle) : Car(x, y, angle), nn({10, 8, 2}) { // 10 capteurs d'entrée, 8 neurones cachés, 2 sorties (volant et pédales)
    initializeSensors(90.0f, 10, 500.0f); 
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

void AICar::updateAI(const Track& track) {
    if (isDead) return;

    updateSensors(track); 

    
    std::vector<float> sensorData;
    for (const Sensor& s : sensors) {
        sensorData.push_back(s.readValue); // On envoie les valeurs (0.0 à 1.0)
    }

    std::vector<float> outputs = nn.feedForward(sensorData);

    // 4. On transforme la pensée en action mécanique !
    // outputs[0] gère le volant (de -1.0 à 1.0)
    // outputs[1] gère l'accélérateur (de -1.0 à 1.0)
    
    float turnAmount = outputs[0] * 4.0f;     // 4.0f est ta vitesse de rotation max
    float accAmount = outputs[1] * 1.0f;      // 1.0f est ton accélération max

    this->turn(turnAmount);
    this->accelerate(accAmount);

    Car::update();
}