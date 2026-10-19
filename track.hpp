#pragma once

#include <vector>
#include "point.hpp"
#include "line.hpp"
#include "car.hpp"

struct Track {
    std::vector<Line> lines;
    float angleStart;
    Point start;
    Line finish;

    // checkpoints dans l'ordre de passage, la ligne d'arrivee n'en fait pas partie
    std::vector<Line> checkpoints;

    Track() = default;

    Track(std::vector<Line> lines, float angleStart, Point start, Line finish, std::vector<Line> checkpoints = {}) : lines(lines), angleStart(angleStart), start(start), finish(finish), checkpoints(checkpoints) {};

    bool checkWallCollision(const Car& car) const;

    bool checkFinishLine(const Car& car) const;

    bool checkLineCrossed(const Car& car, const Line& line) const;

    // "porte" = checkpoint ou ligne d'arrivee : les checkpoints puis l'arrivee en dernier
    int getGateCount() const;
    const Line& getGate(int index) const;
    Vector2 getGateCenter(int index) const;
};