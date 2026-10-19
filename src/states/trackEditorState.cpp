#include "states/trackEditorState.hpp"
#include "core/app.hpp"
#include "raymath.h"
#include "editor/trackEditor.hpp"
#include "io/trackSerializer.hpp"
#include "io/fileLoader.hpp"
#include "game/track.hpp"
#include "ui/ui.hpp"

TrackEditorState::TrackEditorState() {
    camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { 0, 0 }; // L'origine de la caméra
    camera.target = { 0, 0 }; // La cible de la caméra
}

void TrackEditorState::onEnter(App* app) {
    toolbar = ui::button({.text = "ENREGISTRER", .onClick = [this] { requestSave(); }, .size = {200, 40}});
    toolbar->setPosition({app->W - 210.0f, 10});
}

void TrackEditorState::update(App* app) {
    (void)app;

    // lu a chaque frame meme sans popup, pour vider la file des caracteres tapes
    // (sinon les touches WASD tapees avant apparaitraient dans le champ du nom)
    ui::Input input = ui::Input::poll();

    // changements demandes par les callbacks a la frame precedente
    if (!savePopupOpen) {
        savePopup.reset();
    } else if (savePopupDirty) {
        buildSavePopup();
        savePopupDirty = false;
    }

    if (messageTimer > 0.0f) {
        messageTimer -= GetFrameTime();
    }

    // pendant le popup, l'editeur ne recoit aucune entree (les touches tapees servent au nom)
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

    // un clic sur la barre d'outils ne doit pas placer de point dans le circuit
    bool mouseOnToolbar = CheckCollisionPointRec(input.mouse, toolbar->getBounds());
    updateEditor(!mouseOnToolbar);
}

void TrackEditorState::updateEditor(bool mouseAllowed) {
    if(IsKeyDown(KEY_W)) {
        camera.target.y -= 20; // Déplacer la caméra vers le haut
    }
    if(IsKeyDown(KEY_S)) {
        camera.target.y += 20; // Déplacer la caméra vers le bas
    }
    if(IsKeyDown(KEY_A)) {
        camera.target.x -= 20; // Déplacer la caméra vers la gauche
    }
    if(IsKeyDown(KEY_D)) {
        camera.target.x += 20; // Déplacer la caméra vers la droite
    }

    if (IsKeyPressed(KEY_Q)) {
        camera.zoom *= 1.1f; // Zoomer
    }
    if (IsKeyPressed(KEY_E)) {
        camera.zoom /= 1.1f; // Dézoomer
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

    // apres le placement ci-dessus : le clic qui ferme le trace ne doit pas aussi placer le depart
    editor.handleClick(camera);
}

void TrackEditorState::requestSave() {
    if (!editor.isFinished()) {
        showMessage("Circuit incomplet : ferme le trace, puis place le depart, la direction et l'arrivee", ORANGE);
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
    // le popup est detruit au debut de la frame suivante, pas pendant son propre callback
    savePopupOpen = false;
}

void TrackEditorState::confirmSave() {
    bool useDefaultName = trackName.empty();
    std::string name = useDefaultName ? nextDefaultTrackName() : trackName;

    if (!useDefaultName && trackExists(name)) {
        saveError = "Un circuit \"" + name + "\" existe deja";
        savePopupDirty = true;
        return;
    }

    if (!saveTrack(editor.saveTrack(), name)) {
        saveError = "Impossible d'ecrire le fichier";
        savePopupDirty = true;
        return;
    }

    closeSavePopup();
    showMessage("Circuit enregistre : " + name + ".txt", GREEN);
}

void TrackEditorState::buildSavePopup() {
    std::string defaultName = nextDefaultTrackName();
    bool hasError = !saveError.empty();

    savePopup = ui::modal({}, ui::column({.spacing = 20},
        ui::label({.text = "Enregistrer le circuit", .fontSize = 30}),
        ui::textInput({
            .value = trackName,
            .placeholder = defaultName,
            .onChange = [this](const std::string& value) { trackName = value; },
            .onSubmit = [this] { confirmSave(); },
            .filter = fileNameCharFilter,
            .autoFocus = true,
        }),
        ui::label({
            .text = hasError ? saveError : "Laisser vide pour : " + defaultName,
            .fontSize = 18,
            .color = hasError ? ui::theme::accent : ui::theme::border,
        }),
        ui::row({.spacing = 20},
            ui::button({.text = "ANNULER", .onClick = [this] { closeSavePopup(); }, .size = {170, 50}}),
            ui::button({.text = "VALIDER", .onClick = [this] { confirmSave(); }, .size = {170, 50}})
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

    // mettre cela dans le renderer (flemme pour l'instant)
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
