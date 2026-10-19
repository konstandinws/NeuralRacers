#pragma once

#include "game/point.hpp"
#include "game/line.hpp"
#include "raylib.h"
#include <vector>
#include <queue>
#include <optional>
#include "game/track.hpp"

struct TrackEditor {
    std::vector<Point> points{};
    std::vector<Line> lines{};
    std::queue<Point> q;
    int closeRadius = 20;
    bool isComplete = false;
    std::vector<Point> finishLine{};

    // checkpoints places dans l'ordre de passage, 2 clics par checkpoint
    std::vector<Line> checkpoints{};
    std::optional<Point> pendingCheckpoint; // premier point du checkpoint en cours

    // utiliser des smarts_pointers pour eviter le destructeur
    Point* start = nullptr;
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
    void placeAngleStart(const Camera2D& camera);
    void placeFinish(const Camera2D& camera);
    void placeCheckpoint(const Camera2D& camera);
    void removeLastCheckpoint();
    Track saveTrack();

public:
    bool isFinished();
    void draw();
};

