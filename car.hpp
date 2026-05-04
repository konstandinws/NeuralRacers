#pragma once

#include "raylib.h"
#include "point.hpp"
#include "line.hpp"

struct Car {
    int width;
    int height;
    int w2;
    int h2; 
    float angle;
    float rad;
    Vector2 pos;
    Vector2 velocity;
    float friction;

    Rectangle carRect;
    Vector2 origin;
    Point a;
    Point b;
    Point c;
    Point d;

    Line haut;
    Line gauche;
    Line bas;
    Line droite;

    Line carLines[4];


    Car(float x, float y);

    void updateCollisionLines();
    void update();
    void drawCollision();
    void handleInput();
};