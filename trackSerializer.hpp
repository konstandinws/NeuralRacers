#include <fstream>
#include "track.hpp"

void saveTrack(const Track& track);

void loadTrack(const std::string& filename, Track& track);

void parseLine(std::string& line, std::vector<Point>& points, std::vector<Line>& lines);