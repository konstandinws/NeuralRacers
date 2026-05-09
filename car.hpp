#pragma once

#include "raylib.h"
#include "point.hpp"
#include "line.hpp"

struct Car {
    Vector2 pos;
    Vector2 velocity;
    float angle;
    float rad;
    float friction;
    
    int width, height;

    Line carLines[4]; 

    Car() = default;
    Car(float x, float y, float angle);

    // utile pour par lier le mouvement de la voiture au clavier
    void turn(float amount);
    void accelerate(float amount);

    void update();
    void updateCollisionLines();
    void drawCollision();

    void setStartPosition(float x, float y);
    void setStartAngle(float angle);
    void reset(float x, float y, float angle);
};