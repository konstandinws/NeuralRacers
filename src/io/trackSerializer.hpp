#pragma once
#include <fstream>
#include <optional>
#include <string>
#include "game/track.hpp"

// tracks folder, relative to the folder the game is launched from (project root)
inline const std::string TRACKS_DIR = "assets/tracks/";

// saves to TRACKS_DIR/<name>.txt, returns false if the file could not be written
bool saveTrack(const Track& track, const std::string& name);

bool trackExists(const std::string& name);

// first free name of the form new_track_X, X = highest existing number + 1
std::string nextDefaultTrackName();

// Reads a track file. Returns nullopt when the file is missing or malformed;
// if error is given, it receives a short message that can be shown to the player.
std::optional<Track> loadTrack(const std::string& path, std::string* error = nullptr);
