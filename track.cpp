#include "track.hpp"
#include "raylib.h"

Track::Track() {}

Track::Track(std::vector<Point> points, std::vector<Line> lines) : points(points), lines(lines){}
