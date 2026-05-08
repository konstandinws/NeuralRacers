#include <fstream>
#include "track.hpp"

void saveTrack(const Track& track);

Track loadTrack(const std::string& filename);

void parseLine(std::string& line, std::vector<Line>& lines);

void parseStart(std::string& line, Point& start);

void parseAngleStart(std::string& line, float& angleStart);

void parseFinish(std::string& line, Line& finish);