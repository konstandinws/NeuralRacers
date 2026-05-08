#include "trackSerializer.hpp"
#include <string>

void saveTrack(const Track& track) {

    if(track.isFinished) {
        std::fstream file("track.txt", std::ios::out);

        for(Line l : track.lines) {
            file << l.a.x << "," << l.a.y << "-" << l.b.x << "," << l.b.y << std::endl;
        }
        file << "<EOF>";

        file.close();
    }
}

Track loadTrack(const std::string& filename) {

    std::vector<Point> points;
    std::vector<Line> lines;

    std::fstream file(filename, std::ios::in);

    std::string line;

    while(std::getline(file, line)) {
        if (line == "<EOF>") {
            break;
        }
        parseLine(line, points, lines);
    }

    file.close();

    return Track(points, lines);
}

void parseLine(std::string& line, std::vector<Point>& points, std::vector<Line>& lines) {
    int dash = line.find("-");
    std::string subString1 = line.substr(0, dash);
    std::string subString2 = line.substr(dash + 1);

    int comma = subString1.find(",");
    std::string x1 = subString1.substr(0, comma);
    std::string y1 = subString1.substr(comma + 1);

    comma = subString2.find(",");
    std::string x2 = subString2.substr(0, comma);
    std::string y2 = subString2.substr(comma + 1);

    // pue la merde car on ajoute 2 fois les memes points
    // peut etre enlever les points et ne faire que des lignes
    Point a = {std::stof(x1), std::stof(y1)};
    Point b = {std::stof(x2), std::stof(y2)};

    // fix un peu degeu mais ca fait l'affaire pour eviter les doublons de points
    if (points.size() == 0) {
        points.push_back(a);
    }

    points.push_back(b);
    lines.push_back(Line{a, b});
}