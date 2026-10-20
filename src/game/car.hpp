#pragma once

#include "raylib.h"
#include "game/point.hpp"
#include "game/line.hpp"

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

    // used to bind the car's movement to the keyboard
    void turn(float amount);
    void accelerate(float amount);

    void update();
    void updateCollisionLines();
    void drawCollision();

    void setStartPosition(float x, float y);
    void setStartAngle(float angle);
    void reset(float x, float y, float angle);
};