#include "renderer.hpp"

void Renderer::drawCar(Car* car) {
    DrawRectanglePro(car->carRect, car->origin, car-> angle, RED);
}

void Renderer::showCarInfos(Car* car) {
    DrawText(TextFormat("Position: (%.2f, %.2f)", car->pos.x, car->pos.y), 10, 10, 20, WHITE);
    DrawText(TextFormat("Angle: %.2f", car->angle), 10, 40, 20, WHITE);
    DrawText(TextFormat("Velocity: (%.2f, %.2f)", car->velocity.x, car->velocity.y), 10, 70, 20, WHITE);
}

void Renderer::showBindings() {
    
}
