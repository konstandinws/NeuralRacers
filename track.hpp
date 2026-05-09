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

    Track() = default;

    Track(std::vector<Line> lines, float angleStart, Point start, Line finish) : lines(lines), angleStart(angleStart), start(start), finish(finish) {};

    bool checkWallCollision(const Car& car);

    bool checkFinishLine(const Car& car);
};