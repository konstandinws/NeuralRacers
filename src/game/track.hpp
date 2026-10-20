#pragma once

#include <vector>
#include "game/point.hpp"
#include "game/line.hpp"
#include "game/car.hpp"

struct Track {
    std::vector<Line> lines;
    float angleStart;
    Point start;
    Line finish;

    // checkpoints in driving order, the finish line is not one of them
    std::vector<Line> checkpoints;

    Track() = default;

    Track(std::vector<Line> lines, float angleStart, Point start, Line finish, std::vector<Line> checkpoints = {}) : lines(lines), angleStart(angleStart), start(start), finish(finish), checkpoints(checkpoints) {};

    bool checkWallCollision(const Car& car) const;

    bool checkFinishLine(const Car& car) const;

    bool checkLineCrossed(const Car& car, const Line& line) const;

    // "gate" = checkpoint or finish line: the checkpoints, then the finish last
    int getGateCount() const;
    const Line& getGate(int index) const;
    Vector2 getGateCenter(int index) const;
};