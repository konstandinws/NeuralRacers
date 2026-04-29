#pragma once

#include "point.hpp"
#include "line.hpp"
#include <vector>
#include <queue>
#include "track.hpp"

struct TrackEditor {
    std::vector<Point> points{};
    std::vector<Line> lines{};
    std::queue<Point> q;
    int closeRadius;

    TrackEditor();

    void handleClick();
    Track getTrack();
    void draw();
};

