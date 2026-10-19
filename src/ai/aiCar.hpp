#pragma once

#include "game/car.hpp"
#include <vector>
#include "game/track.hpp"
#include "ai/neuralNetwork.hpp"

inline constexpr int SENSOR_COUNT = 10;

// entrees du reseau : les 10 capteurs, puis la vitesse vers l'avant et la vitesse laterale (derapage)
// 8 neurones caches, 2 sorties (volant et pedales)
// un modele sauvegarde ne peut etre recharge que s'il a la meme topologie
// (les anciens modeles a 10 entrees sont adaptes au chargement, voir NeuralNetwork::widenInputs)
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

    float MAX_TURN = 4.0f;          // degres par frame quand le volant est a fond
    float MAX_ACCELERATION = 1.0f;  // pixels par frame ajoutes a la vitesse quand la pedale est a fond

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
    
    float MIN_SPEED = 5.0f;    // minimum speed in pixels per FRAME (top speed is ~9 px/frame)
    float MAX_SLOW_TIME = 5.0f;  // Temps maximum autorisé en sous-régime
    float GRACE_PERIOD = 1.0f;   // Temps laissé au départ pour accélérer

    AICar(float x, float y, float angle);
    void updateSensors(const Track& track);
    // entrees du reseau, dans l'ordre de AI_TOPOLOGY : capteurs, vitesse avant, vitesse laterale
    std::vector<float> computeInputs() const;
    void updateAI(const Track& track);
    void updateProgress(const Track& track);
    void drawSensors();
    void initializeSensors(float fov, int numRays, float maxDistance);
};