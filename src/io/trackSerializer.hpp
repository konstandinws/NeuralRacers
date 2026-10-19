#pragma once
#include <fstream>
#include <optional>
#include <string>
#include "game/track.hpp"

// dossier des circuits, relatif au dossier depuis lequel le jeu est lance (racine du projet)
inline const std::string TRACKS_DIR = "assets/tracks/";

// enregistre dans TRACKS_DIR/<name>.txt, renvoie false si le fichier n'a pas pu etre ecrit
bool saveTrack(const Track& track, const std::string& name);

bool trackExists(const std::string& name);

// premier nom libre de la forme nouveau_circuit_X, X = plus grand numero existant + 1
std::string nextDefaultTrackName();

// Reads a track file. Returns nullopt when the file is missing or malformed;
// if error is given, it receives a short message that can be shown to the player.
std::optional<Track> loadTrack(const std::string& path, std::string* error = nullptr);
