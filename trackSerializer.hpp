#include <fstream>
#include "track.hpp"

void saveTrack(const Track& track);

Track loadTrack(const std::string& filename);

void parseLine(std::string& line, std::vector<Point>& points, std::vector<Line>& lines);