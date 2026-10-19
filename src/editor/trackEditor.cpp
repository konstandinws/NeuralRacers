#include "editor/trackEditor.hpp"
#include "game/track.hpp"
#include <iostream>
#include <cmath>

void TrackEditor::handleClick(const Camera2D& camera)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isComplete)
    {
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        Point p = {mousePos.x, mousePos.y};

        if (points.size() > 3)
        {
            Point first = points.front();
            if (pow(first.x - mousePos.x, 2) <= pow(closeRadius, 2) &&
                pow(first.y - mousePos.y, 2) <= pow(closeRadius, 2))
            {
                p.x = first.x;
                p.y = first.y;
                isComplete = true;
            }
        }
        points.push_back(p);
        q.push(p);
        if (q.size() > 1)
        {
            lines.push_back(Line{q.front(), q.back()});
            q.pop();
        }
    }

    // fonction de debug pour afficher les points et les lignes 
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        std::cout << "Track EDITOR  \n";
        std::cout << "Points : ";
        for (Point p : points)
        {
            std::cout << "(" << p.x << ", " << p.y << ") ";
        }
        std::cout << "\n";

        std::cout << "Lines : ";
        for (Line l : lines)
        {
            std::cout << "(" << l.a.x << ", " << l.a.y << ")" << ", " << "(" << l.b.x << ", " << l.b.y << ") ";
        }
        std::cout << "\n";

        if(start) {
            std::cout << "Start : (" << start->x << ", " << start->y << ")\n";
        }

        std::cout << "Finish Line : ";
        for (Point p : finishLine)
        {
            std::cout << "(" << p.x << ", " << p.y << ") ";
        }
        std::cout << "\n";

        if(angleStart) {
            std::cout << "Angle Start : " << *angleStart << "\n";
        }
    }
}

void TrackEditor::placeStart(const Camera2D& camera)
{
    if(!start && isComplete) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            start = Point{mousePos.x, mousePos.y};
        }
    }
}

void TrackEditor::placeAngleStart(const Camera2D& camera)
{
    if(!angleStart && start) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            float angle = atan2(mousePos.y - start->y, mousePos.x - start->x);
            angleStart = angle;
        }
    }
}

void TrackEditor::placeFinish(const Camera2D& camera)
{
    // isComplete pour etre sur, on verifie deja avant
    if(!finish && finishLine.size() < 2 && isComplete) {
         if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            finishLine.push_back(Point{mousePos.x, mousePos.y});
            if(finishLine.size() == 2) {
                finish = Line{finishLine[0], finishLine[1]};
            }
        }
    }
}

void TrackEditor::clear()
{
    points.clear();
    lines.clear();
    while (!q.empty()) {
        q.pop();
    }
    isComplete = false;

    start.reset();
    finish.reset();
    finishLine.clear();
    angleStart.reset();

    checkpoints.clear();
    pendingCheckpoint.reset();
}

void TrackEditor::placeCheckpoint(const Camera2D& camera)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        Point p = {mousePos.x, mousePos.y};

        if (!pendingCheckpoint) {
            pendingCheckpoint = p;
        } else {
            checkpoints.push_back(Line{*pendingCheckpoint, p});
            pendingCheckpoint.reset();
        }
    }
}

void TrackEditor::removeLastCheckpoint()
{
    // annule d'abord le checkpoint en cours, sinon le dernier checkpoint complet
    if (pendingCheckpoint) {
        pendingCheckpoint.reset();
    } else if (!checkpoints.empty()) {
        checkpoints.pop_back();
    }
}

Track TrackEditor::saveTrack()
{
    return Track(lines, *angleStart, *start, *finish, checkpoints);
}

bool TrackEditor::isFinished() {
    if(isComplete && start && angleStart && finish) {
        return true;
    }
    return false;
}

void TrackEditor::draw()
{
    for (Point p : points)
    {
        DrawCircle(p.x, p.y, 5, YELLOW);
    }

    for (Line l : lines)
    {
        DrawLine(l.a.x, l.a.y, l.b.x, l.b.y, GREEN);
    }

    if(start) {
        DrawCircle(start->x, start->y, 7, BLUE);
    }

    if(finish) {
        DrawLine(finish->a.x, finish->a.y, finish->b.x, finish->b.y, RED);
    }

    // numero de chaque checkpoint pour verifier l'ordre de passage
    for (size_t i = 0; i < checkpoints.size(); i++)
    {
        const Line& c = checkpoints[i];
        DrawLine(c.a.x, c.a.y, c.b.x, c.b.y, SKYBLUE);
        DrawText(TextFormat("%d", (int)i + 1), (c.a.x + c.b.x) / 2.0f + 5, (c.a.y + c.b.y) / 2.0f + 5, 20, SKYBLUE);
    }

    if (pendingCheckpoint) {
        DrawCircle(pendingCheckpoint->x, pendingCheckpoint->y, 5, SKYBLUE);
    }

    
}