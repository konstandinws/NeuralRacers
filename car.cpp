#include "car.hpp"
#include <cmath>

Car::Car(float x, float y) : pos({x, y}), velocity({0, 0}), angle(0), friction(0.1f), width(20), height(50) {
    rad = angle * DEG2RAD;
    updateCollisionLines();
}

void Car::turn(float amount) {
    angle += amount;
    rad = angle * DEG2RAD;
}

void Car::accelerate(float amount) {
    velocity.x += amount * sin(rad);
    velocity.y -= amount * cos(rad); 
}

void Car::update() {
    velocity.x *= 1.0f - friction;
    velocity.y *= 1.0f - friction;

    pos.x += velocity.x;
    pos.y += velocity.y;

    updateCollisionLines();
}

void Car::updateCollisionLines() {
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    
    Point p1 = {pos.x - w2 * cos_a - (-h2) * sin_a, pos.y - w2 * sin_a + (-h2) * cos_a};
    Point p2 = {pos.x + w2 * cos_a - (-h2) * sin_a, pos.y + w2 * sin_a + (-h2) * cos_a};
    Point p3 = {pos.x + w2 * cos_a - h2 * sin_a, pos.y + w2 * sin_a + h2 * cos_a};
    Point p4 = {pos.x - w2 * cos_a - h2 * sin_a, pos.y - w2 * sin_a + h2 * cos_a};
    
    carLines[0] = {p1, p2}; // haut
    carLines[1] = {p2, p3}; // droite
    carLines[2] = {p3, p4}; // bas
    carLines[3] = {p4, p1}; // gauche
}

// fonction plus pour le debug
void Car::drawCollision() {
    for(Line& l : carLines){
        DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, GREEN);
    }
}

void Car::reset(float x, float y) {
    pos = {x, y};
    velocity = {0.0f, 0.0f};
    angle = 0;
    rad = 0;
    updateCollisionLines();
}