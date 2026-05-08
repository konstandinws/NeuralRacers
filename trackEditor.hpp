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
    int closeRadius = 20;
    bool isComplete = false;
    std::vector<Point> finishLine{};
    
    // utiliser des smarts_pointers pour eviter le destructeur
    Vector2* start = nullptr;
    Line* finish = nullptr;
    float* angleStart = nullptr;

    ~TrackEditor() {
        delete start;
        delete finish;
        delete angleStart;
    }


    void handleClick(const Camera2D& camera);
    void clear();
    void placeStart(const Camera2D& camera);
    void placeFinish(const Camera2D& camera);
    Track getTrack();

public:
    bool isFinished();

    void draw();
};

