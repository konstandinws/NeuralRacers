#pragma once

#include "game/car.hpp"
#include <vector>
#include "game/track.hpp"
#include "ai/neuralNetwork.hpp"

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

    // progression sur le circuit : index de la prochaine porte a franchir (checkpoints puis arrivee)
    // progress = portes franchies + avancement (0 a 1) vers la suivante
    int nextGate = 0;
    float progress = 0.0f;
    bool hasFinished = false;

    float GATE_SCORE = 1000.0f;   // points par porte franchie
    float TIME_PENALTY = 10.0f;   // points perdus par seconde, pour favoriser les plus rapides

    // Pour evite que les voitures fassent du surplace
    float bestProgress = 0.0f;
    float stagnationCounter = 0.0f;
    float stagnationLimit = 3.0f;

    // pour eviter que les voitures roulent lentement pendant trop longtemps sans mourir
    float aliveTimer = 0.0f;       // Depuis combien de temps la voiture est en vie ?
    float lowSpeedTimer = 0.0f;    // Depuis combien de temps roule-t-elle trop doucement ?
    
    float MIN_SPEED = 5.0f;    // Vitesse minimum exigée (en pixels par seconde)
    float MAX_SLOW_TIME = 5.0f;  // Temps maximum autorisé en sous-régime
    float GRACE_PERIOD = 1.0f;   // Temps laissé au départ pour accélérer

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    void updateAI(const Track& track);
    void updateProgress(const Track& track);
    void drawSensors();
    void initializeSensors(float fov, int numRays, float maxDistance);
};