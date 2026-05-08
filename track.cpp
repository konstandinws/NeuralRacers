#include "track.hpp"
#include "raylib.h"

Track::Track() : isVisible(false), isFinished(false) {}

Track::Track(std::vector<Point> points, std::vector<Line> lines) 
    : points(points), lines(lines), isVisible(false), isFinished(true) {}
