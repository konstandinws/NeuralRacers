#pragma once
#include <fstream>
#include <string>
#include "game/track.hpp"

// dossier des circuits, relatif au dossier depuis lequel le jeu est lance (racine du projet)
inline const std::string TRACKS_DIR = "assets/tracks/";

// enregistre dans TRACKS_DIR/<name>.txt, renvoie false si le fichier n'a pas pu etre ecrit
bool saveTrack(const Track& track, const std::string& name);

bool trackExists(const std::string& name);

// premier nom libre de la forme nouveau_circuit_X, X = plus grand numero existant + 1
std::string nextDefaultTrackName();

Track loadTrack(const std::string& filename);

void parseLine(std::string& line, std::vector<Line>& lines);

void parseStart(std::string& line, Point& start);

void parseAngleStart(std::string& line, float& angleStart);

void parseFinish(std::string& line, Line& finish);