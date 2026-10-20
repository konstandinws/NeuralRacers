#include "states/trackEditorState.hpp"
#include "core/app.hpp"
#include "raymath.h"
#include "editor/trackEditor.hpp"
#include "io/trackSerializer.hpp"
#include "io/fileLoader.hpp"
#include "game/track.hpp"
#include "ui/ui.hpp"

TrackEditorState::TrackEditorState() {
    camera = {};
    camera.zoom = 1.0f;
    camera.offset = { 0, 0 }; // camera origin
    camera.target = { 0, 0 }; // camera target
}

void TrackEditorState::onEnter(App* app) {
    toolbar = ui::button({.text = "SAVE", .onClick = [this] { requestSave(); }, .size = {200, 40}});
    toolbar->setPosition({app->W - 210.0f, 10});
}

void TrackEditorState::update(App* app) {
    (void)app;

    // read every frame even without the popup, to drain the typed character queue
    // (otherwise WASD keys typed earlier would show up in the name field)
    ui::Input input = ui::Input::poll();

    // changes requested by callbacks during the previous frame
    if (!savePopupOpen) {
        savePopup.reset();
    } else if (savePopupDirty) {
        buildSavePopup();
        savePopupDirty = false;
    }

    if (messageTimer > 0.0f) {
        messageTimer -= GetFrameTime();
    }

    // while the popup is open, the editor gets no input (typed keys go to the name)
    if (savePopupOpen) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            closeSavePopup();
        } else {
            savePopup->update(input);
        }
        return;
    }

    toolbar->update(input);

    if (IsKeyPressed(KEY_G)) {
        requestSave();
        return;
    }

    // a click on the toolbar must not place a point on the track
    bool mouseOnToolbar = CheckCollisionPointRec(input.mouse, toolbar->getBounds());
    updateEditor(!mouseOnToolbar);
}

void TrackEditorState::updateEditor(bool mouseAllowed) {
    if(IsKeyDown(KEY_W)) {
        camera.target.y -= 20; // move the camera up
    }
    if(IsKeyDown(KEY_S)) {
        camera.target.y += 20; // move the camera down
    }
    if(IsKeyDown(KEY_A)) {
        camera.target.x -= 20; // move the camera left
    }
    if(IsKeyDown(KEY_D)) {
        camera.target.x += 20; // move the camera right
    }

    if (IsKeyPressed(KEY_Q)) {
        camera.zoom *= 1.1f; // zoom in
    }
    if (IsKeyPressed(KEY_E)) {
        camera.zoom /= 1.1f; // zoom out
    }

    if(IsKeyPressed(KEY_SPACE)) {
        editor.clear();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        editor.removeLastCheckpoint();
    }

    if (!mouseAllowed) {
        return;
    }

    if(editor.isComplete) {
        if(!editor.start) {
            editor.placeStart(camera);
        } else if(!editor.angleStart) {
            editor.placeAngleStart(camera);
        } else if(!editor.finish) {
            editor.placeFinish(camera);
        } else {
            editor.placeCheckpoint(camera);
        }
    }

    // after the placement above: the click that closes the outline must not also place the start
    editor.handleClick(camera);
}

void TrackEditorState::requestSave() {
    if (!editor.isFinished()) {
        showMessage("Incomplete track: close the outline, then place the start, direction and finish line", ORANGE);
        return;
    }
    openSavePopup();
}

void TrackEditorState::openSavePopup() {
    savePopupOpen = true;
    trackName.clear();
    saveError.clear();
    buildSavePopup();
}

void TrackEditorState::closeSavePopup() {
    // the popup is destroyed at the start of the next frame, not during its own callback
    savePopupOpen = false;
}

void TrackEditorState::confirmSave() {
    bool useDefaultName = trackName.empty();
    std::string name = useDefaultName ? nextDefaultTrackName() : trackName;

    if (!useDefaultName && trackExists(name)) {
        saveError = "A track named \"" + name + "\" already exists";
        savePopupDirty = true;
        return;
    }

    if (!saveTrack(editor.saveTrack(), name)) {
        saveError = "Could not write the file";
        savePopupDirty = true;
        return;
    }

    closeSavePopup();
    showMessage("Track saved: " + name + ".txt", GREEN);
}

void TrackEditorState::buildSavePopup() {
    std::string defaultName = nextDefaultTrackName();
    bool hasError = !saveError.empty();

    savePopup = ui::modal({}, ui::column({.spacing = 20},
        ui::label({.text = "Save track", .fontSize = 30}),
        ui::textInput({
            .value = trackName,
            .placeholder = defaultName,
            .onChange = [this](const std::string& value) { trackName = value; },
            .onSubmit = [this] { confirmSave(); },
            .filter = fileNameCharFilter,
            .autoFocus = true,
        }),
        ui::label({
            .text = hasError ? saveError : "Leave empty for: " + defaultName,
            .fontSize = 18,
            .color = hasError ? ui::theme::accent : ui::theme::border,
        }),
        ui::row({.spacing = 20},
            ui::button({.text = "CANCEL", .onClick = [this] { closeSavePopup(); }, .size = {170, 50}}),
            ui::button({.text = "CONFIRM", .onClick = [this] { confirmSave(); }, .size = {170, 50}})
        )
    ));
}

void TrackEditorState::showMessage(const std::string& text, Color color) {
    message = text;
    messageColor = color;
    messageTimer = 3.0f;
}

void TrackEditorState::render(App* app) {
    DrawText("Track Editor - WASD to move, Q/E to zoom, SPACE to clear, G to save", 10, 10, 20, RAYWHITE);
    if (editor.finish) {
        DrawText("Checkpoints: 2 clicks each, in driving order - BACKSPACE to undo", 10, 35, 20, SKYBLUE);
    }
    BeginMode2D(camera);

    // should move to the renderer (later)
    editor.draw();
    if(!editor.angleStart && editor.start) {
        DrawLine(editor.start->x, editor.start->y, GetScreenToWorld2D(GetMousePosition(), camera).x, GetScreenToWorld2D(GetMousePosition(), camera).y, BLUE);
    }
    if(editor.angleStart) {
        DrawLine(editor.start->x, editor.start->y, editor.start->x + cos(*editor.angleStart) * 50, editor.start->y + sin(*editor.angleStart) * 50, BLUE);
    }
    if(editor.pendingCheckpoint) {
        Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
        DrawLine(editor.pendingCheckpoint->x, editor.pendingCheckpoint->y, mouse.x, mouse.y, SKYBLUE);
    }
    EndMode2D();

    toolbar->draw();

    if (messageTimer > 0.0f) {
        int width = MeasureText(message.c_str(), 20);
        DrawText(message.c_str(), (app->W - width) / 2, app->H - 50, 20, messageColor);
    }

    if (savePopupOpen && savePopup) {
        savePopup->draw();
    }
}
