#include "track.hpp"
#include "raylib.h"

Track::Track() : isVisible(false), isFinished(false) {}

Track::Track(std::vector<Point> points, std::vector<Line> lines) 
    : points(points), lines(lines), isVisible(false), isFinished(true) {}


void Track::draw() {
    for(Line l : lines) {
        DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, BLUE);
    }
}