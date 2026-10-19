#pragma once
#include <fstream>
#include <string>
#include "game/track.hpp"

// dossier des circuits, relatif au dossier depuis lequel le jeu est lance (racine du projet)
inline const std::string TRACKS_DIR = "assets/tracks/";

void saveTrack(const Track& track);

Track loadTrack(const std::string& filename);

void parseLine(std::string& line, std::vector<Line>& lines);

void parseStart(std::string& line, Point& start);

void parseAngleStart(std::string& line, float& angleStart);

void parseFinish(std::string& line, Line& finish);