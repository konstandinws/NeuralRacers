#include "trackEditor.hpp"
#include "track.hpp"
#include <iostream>
#include <cmath>

TrackEditor::TrackEditor() {
    closeRadius = 20;
}

void TrackEditor::handleClick(const Camera2D& camera)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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
                std::cout << "Circuit done" << "\n";
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
    }
}


void TrackEditor::clear()
{
    points.clear();
    lines.clear();
    while (!q.empty()) {
        q.pop();
    }
}

Track TrackEditor::getTrack()
{
    return Track{points, lines};
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
}