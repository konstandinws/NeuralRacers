#include "trackEditor.hpp"
#include "track.hpp"
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

        if(start != nullptr) {
            std::cout << "Start : (" << start->x << ", " << start->y << ")\n";
        }

        std::cout << "Finish Line : ";
        for (Point p : finishLine)
        {
            std::cout << "(" << p.x << ", " << p.y << ") ";
        }
        std::cout << "\n";
    }
}

void TrackEditor::placeStart(const Camera2D& camera)
{
    if(start == nullptr && isComplete) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            start = new Vector2(mousePos);
        }
    }
}

void TrackEditor::placeAngleStart(const Camera2D& camera)
{
    if(angleStart == nullptr && start != nullptr) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            float angle = atan2(mousePos.y - start->y, mousePos.x - start->x);
            angleStart = new float(angle);
        }
    }
}

void TrackEditor::placeFinish(const Camera2D& camera)
{
    // isComplete pour etre sur, on verifie deja avant
    if(finish == nullptr && finishLine.size() < 2 && isComplete) {
         if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            finishLine.push_back(Point{mousePos.x, mousePos.y});
            if(finishLine.size() == 2) {
                finish = new Line{finishLine[0], finishLine[1]};
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

    // degueulasse car j'utilise les pointeurs bruts
    delete start;
    start = nullptr;
    delete finish;
    finish = nullptr;
    finishLine.clear();
    delete angleStart;
    angleStart = nullptr;
}

Track TrackEditor::getTrack()
{
    return Track{points, lines};
}

bool TrackEditor::isFinished() {
    if(isComplete && start != nullptr && finish != nullptr) {
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

    if(start != nullptr) {
        DrawCircle(start->x, start->y, 7, BLUE);
    }

    if(finish != nullptr) {
        DrawLine(finish->a.x, finish->a.y, finish->b.x, finish->b.y, RED);
    }

    
}