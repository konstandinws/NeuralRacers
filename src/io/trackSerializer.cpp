#include "io/trackSerializer.hpp"
#include "io/fileLoader.hpp"
#include <string>
#include <filesystem>

static std::string trackPath(const std::string& name) {
    return TRACKS_DIR + name + ".txt";
}

bool trackExists(const std::string& name) {
    return std::filesystem::exists(trackPath(name));
}

std::string nextDefaultTrackName() {
    return nextFreeName(TRACKS_DIR, "nouveau_circuit_");
}

bool saveTrack(const Track& track, const std::string& name) {

    // j'ai enlever le if isFinished() car on peut pas enregister un track qui n'est pas fini

    std::fstream file(trackPath(name), std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    file << "S " << track.start.x << "," << track.start.y << std::endl;
    file << "A " << track.angleStart << std::endl;
    file << "F " << track.finish.a.x << "," << track.finish.a.y << ";" << track.finish.b.x << "," << track.finish.b.y << std::endl;

    // les checkpoints sont ecrits dans l'ordre de passage
    for (const Line& c : track.checkpoints)
    {
        file << "C " << c.a.x << "," << c.a.y << ";" << c.b.x << "," << c.b.y << std::endl;
    }

    for (Line l : track.lines)
    {
        file << l.a.x << "," << l.a.y << ";" << l.b.x << "," << l.b.y << std::endl;
    }
    file << "<EOF>";

    file.close();
    return !file.fail();
}

Track loadTrack(const std::string& filename) {

    std::vector<Line> lines;
    float angleStart;
    Point start;
    Line finish;
    std::vector<Line> checkpoints;

    std::fstream file(filename, std::ios::in);

    std::string line;

    while(std::getline(file, line)) {
        if (line == "<EOF>") {
            break;
        } else if (line[0] == 'S') {
            parseStart(line, start);
        } else if (line[0] == 'A') {
            parseAngleStart(line, angleStart);
        } else if (line[0] == 'F') {
            parseFinish(line, finish);
        } else if (line[0] == 'C') {
            // meme format que la ligne d'arrivee : "C x1,y1;x2,y2"
            Line checkpoint;
            parseFinish(line, checkpoint);
            checkpoints.push_back(checkpoint);
        } else {
            parseLine(line, lines);
        }
    }

    file.close();

    return Track(lines, angleStart, start, finish, checkpoints);
}

void parseLine(std::string& line, std::vector<Line>& lines) {
    int dash = line.find(";");
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

    lines.push_back(Line{a, b});
}

void parseStart(std::string& line, Point& start) {
    std::string subString = line.substr(2);
    int comma = subString.find(",");
    std::string x = subString.substr(0, comma);
    std::string y = subString.substr(comma + 1);
    start = {std::stof(x), std::stof(y)};
}

void parseAngleStart(std::string& line, float& angleStart) {
    std::string subString = line.substr(2);
    angleStart = std::stof(subString);
}

void parseFinish(std::string& line, Line& finish) {
    std::string subString = line.substr(2);
    int dash = subString.find(";");
    std::string subString1 = subString.substr(0, dash);
    std::string subString2 = subString.substr(dash + 1);

    int comma = subString1.find(",");
    std::string x1 = subString1.substr(0, comma);
    std::string y1 = subString1.substr(comma + 1);

    comma = subString2.find(",");
    std::string x2 = subString2.substr(0, comma);
    std::string y2 = subString2.substr(comma + 1);

    Point a = {std::stof(x1), std::stof(y1)};
    Point b = {std::stof(x2), std::stof(y2)};

    finish = Line{a, b};
}