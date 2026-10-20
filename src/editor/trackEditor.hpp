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

    // checkpoints placed in driving order, 2 clicks per checkpoint
    std::vector<Line> checkpoints{};
    std::optional<Point> pendingCheckpoint; // first point of the checkpoint being placed

    // empty until placed: no raw pointers, so no destructor and copying is safe (rule of zero)
    std::optional<Point> start;
    std::optional<Line> finish;
    std::optional<float> angleStart;  // radians


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

