#include "AICar.hpp"

AICar::AICar(float x, float y, float angle) : Car(x, y, angle) {
    // Initialize sensors with different angles and max distances
    sensors.push_back({-45.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({-30.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({-15.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({  0.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({ 15.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({ 30.0f, 100.0f, 0.0f, {0.0f, 0.0f}});
    sensors.push_back({ 45.0f, 100.0f, 0.0f, {0.0f, 0.0f}});

}


void AICar::drawSensors() {
    for (const auto& sensor : sensors) {
        DrawLineV(pos, Vector2{sensor.endPoint.x, sensor.endPoint.y}, YELLOW);
    }
}