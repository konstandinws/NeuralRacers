#include "track.hpp"

bool Track::checkWallCollision(const Car& car) {
    Vector2 collisionPoint = {0, 0}; // variable temporaire pour stocker le point de collision, pas utilisée dans la logique actuelle
    for(Line l : lines){
        for(Line carLine : car.carLines){
            if(CheckCollisionLines(Vector2{carLine.a.x, carLine.a.y}, Vector2{carLine.b.x, carLine.b.y}, Vector2{l.a.x, l.a.y}, Vector2{l.b.x, l.b.y}, &collisionPoint)){
                return true;
            }
        }   
    }
    return false;
}

bool Track::checkFinishLine(const Car& car){
    Vector2 collisionPoint = {0, 0}; 
    for(Line carLine : car.carLines){
        if(CheckCollisionLines(Vector2{carLine.a.x, carLine.a.y}, Vector2{carLine.b.x, carLine.b.y}, Vector2{finish.a.x, finish.a.y}, Vector2{finish.b.x, finish.b.y}, &collisionPoint)){
            return true;
        }
    }   
    return false;
}

Vector2 Track::getFinishLineCenter() const {
    return { 
        (finish.a.x + finish.b.x) / 2.0f, 
        (finish.a.y + finish.b.y) / 2.0f 
    };
}