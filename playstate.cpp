#include "playState.hpp"
#include "trackSerializer.hpp"


PlayState::PlayState(const std::string& trackName) {
    // Charger le track à partir du nom fourni
    track = loadTrack(trackName);

    car = Car(600, 400);

    camera = { 0 };
    camera.target = car.pos;
    camera.offset = { 600, 400 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

}

void PlayState::update(App* app) {
    car.handleInput();
    car.update();
}

void PlayState::render(App* app) {
    camera.target = car.pos;
    BeginMode2D(camera);
    track.draw();
    car.drawCollision();
    DrawCircle(600, 400, 5, RED);
    EndMode2D();
}