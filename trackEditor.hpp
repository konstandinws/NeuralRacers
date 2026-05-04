#pragma once

#include "point.hpp"
#include "line.hpp"
#include "raylib.h"
#include <vector>
#include <queue>
#include "track.hpp"

struct TrackEditor {
    std::vector<Point> points{};
    std::vector<Line> lines{};
    std::queue<Point> q;
    int closeRadius;

    TrackEditor();

    void handleClick(const Camera2D& camera);
    Track getTrack();
    void draw();
};

