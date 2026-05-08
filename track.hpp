#pragma once

#include <vector>
#include "point.hpp"
#include "line.hpp"

struct Track {
    std::vector<Point> points;
    std::vector<Line> lines;
    bool isVisible;
    bool isFinished;

    Track();

    Track(std::vector<Point> points, std::vector<Line> lines);
};