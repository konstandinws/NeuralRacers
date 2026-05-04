#include "car.hpp"
#include <cmath>

Car::Car(float x, float y) {
    pos = {x, y};
    width = 20;
    height = 50;
    w2 = width/2;
    h2 = height/2;
    angle = 0;
    rad = angle * DEG2RAD;
    velocity = {0.0f, 0.0f};
    friction = 0.1f;

    origin = { (float)width / 2, (float)height / 2 };
    carRect = { pos.x, pos.y, (float)width, (float)height };
    a = {pos.x - w2, pos.y - h2};
    b = {pos.x + w2, pos.y - h2};
    c = {pos.x + w2, pos.y + h2};
    d = {pos.x - w2, pos.y + h2};

    haut = {a, b};
    gauche = {b, c};
    bas = {c, d};
    droite = {d, a};

    carLines[0] = haut;
    carLines[1] = gauche;
    carLines[2] = bas;
    carLines[3] = droite;

    updateCollisionLines();
}

void Car::updateCollisionLines() {
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    
    a = {pos.x - w2 * cos_a - (-h2) * sin_a, pos.y - w2 * sin_a + (-h2) * cos_a};
    b = {pos.x + w2 * cos_a - (-h2) * sin_a, pos.y + w2 * sin_a + (-h2) * cos_a};
    c = {pos.x + w2 * cos_a - h2 * sin_a, pos.y + w2 * sin_a + h2 * cos_a};
    d = {pos.x - w2 * cos_a - h2 * sin_a, pos.y - w2 * sin_a + h2 * cos_a};
    
    haut = {a, b};
    gauche = {b, c};
    bas = {c, d};
    droite = {d, a};
    
    carLines[0] = Car::haut;
    carLines[1] = Car::gauche;
    carLines[2] = Car::bas;
    carLines[3] = Car::droite;
}

void Car::drawCollision()
{
    for(Line& l : carLines){
            DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, GREEN);
    }
}

void Car::update() {
        carRect.x += velocity.x;
        carRect.y += velocity.y;
        pos.x = carRect.x;
        pos.y = carRect.y;
        updateCollisionLines();
    }

void Car::handleInput(){
    
        if(IsKeyDown(KEY_LEFT)){
            angle -= 4;
            rad = angle * DEG2RAD;
        }
        if(IsKeyDown(KEY_RIGHT)){
            angle += 4;
            rad = angle * DEG2RAD;
        }
        if(IsKeyDown(KEY_UP)){
            velocity.x += 1 * sin(rad);
            velocity.y -= 1 * cos(rad); 
        }
        if(IsKeyDown(KEY_DOWN)){
            velocity.x -= 1 * sin(rad);
            velocity.y += 1 * cos(rad); 
        }

        velocity.x *= 1 - friction;
        velocity.y *= 1 - friction;

        Car::update();
    }