#include "track.hpp"

bool Track::checkWallCollision(const Car& car) const {
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

bool Track::checkFinishLine(const Car& car) const {
    return checkLineCrossed(car, finish);
}

bool Track::checkLineCrossed(const Car& car, const Line& line) const {
    Vector2 collisionPoint = {0, 0};
    for(Line carLine : car.carLines){
        if(CheckCollisionLines(Vector2{carLine.a.x, carLine.a.y}, Vector2{carLine.b.x, carLine.b.y}, Vector2{line.a.x, line.a.y}, Vector2{line.b.x, line.b.y}, &collisionPoint)){
            return true;
        }
    }
    return false;
}

int Track::getGateCount() const {
    return (int)checkpoints.size() + 1;
}

const Line& Track::getGate(int index) const {
    if (index < (int)checkpoints.size()) {
        return checkpoints[index];
    }
    return finish;
}

Vector2 Track::getGateCenter(int index) const {
    const Line& gate = getGate(index);
    return {
        (gate.a.x + gate.b.x) / 2.0f,
        (gate.a.y + gate.b.y) / 2.0f
    };
}
