#include "io/trackSerializer.hpp"
#include "io/fileLoader.hpp"
#include <string>
#include <filesystem>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>

static std::string trackPath(const std::string& name) {
    return TRACKS_DIR + name + ".txt";
}

bool trackExists(const std::string& name) {
    return std::filesystem::exists(trackPath(name));
}

std::string nextDefaultTrackName() {
    return nextFreeName(TRACKS_DIR, "new_track_");
}

bool saveTrack(const Track& track, const std::string& name) {

    // no isFinished() check here: an unfinished track cannot be saved anyway

    std::fstream file(trackPath(name), std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    file << "S " << track.start.x << "," << track.start.y << std::endl;
    file << "A " << track.angleStart << std::endl;
    file << "F " << track.finish.a.x << "," << track.finish.a.y << ";" << track.finish.b.x << "," << track.finish.b.y << std::endl;

    // checkpoints are written in driving order
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

// Strict float parsing: the whole text must be a finite number (std::stof threw on bad input
// and silently ignored trailing garbage).
static bool parseFloat(const std::string& text, float& out) {
    size_t begin = text.find_first_not_of(' ');
    size_t end = text.find_last_not_of(' ');
    if (begin == std::string::npos) {
        return false;
    }
    std::string number = text.substr(begin, end - begin + 1);
    char* parsedEnd = nullptr;
    errno = 0;
    out = std::strtof(number.c_str(), &parsedEnd);
    return parsedEnd == number.c_str() + number.size() && errno != ERANGE && std::isfinite(out);
}

// "x,y"
static bool parsePoint(const std::string& text, Point& point) {
    size_t comma = text.find(',');
    return comma != std::string::npos
        && parseFloat(text.substr(0, comma), point.x)
        && parseFloat(text.substr(comma + 1), point.y);
}

// "x1,y1;x2,y2"
static bool parseSegment(const std::string& text, Line& segment) {
    size_t semicolon = text.find(';');
    return semicolon != std::string::npos
        && parsePoint(text.substr(0, semicolon), segment.a)
        && parsePoint(text.substr(semicolon + 1), segment.b);
}

std::optional<Track> loadTrack(const std::string& path, std::string* error) {
    auto fail = [error](const std::string& message) -> std::optional<Track> {
        if (error) {
            *error = message;
        }
        return std::nullopt;
    };

    std::ifstream file(path);
    if (!file.is_open()) {
        return fail("File not found");
    }

    std::vector<Line> lines;
    std::vector<Line> checkpoints;
    Point start{};
    float angleStart = 0.0f;
    Line finish{};
    bool hasStart = false, hasAngle = false, hasFinish = false;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;

        // tolerate Windows line endings, trailing spaces and blank lines
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        if (line == "<EOF>") {
            break;
        }

        // tagged lines are "<letter> <data>", anything else is a wall segment
        bool tagged = line.size() >= 2 && std::isalpha((unsigned char)line[0]) && line[1] == ' ';
        std::string data = tagged ? line.substr(2) : line;
        bool ok = false;

        if (!tagged) {
            Line wall;
            ok = parseSegment(data, wall);
            if (ok) lines.push_back(wall);
        } else if (line[0] == 'S') {
            ok = hasStart = parsePoint(data, start);
        } else if (line[0] == 'A') {
            ok = hasAngle = parseFloat(data, angleStart);
        } else if (line[0] == 'F') {
            ok = hasFinish = parseSegment(data, finish);
        } else if (line[0] == 'C') {
            // same format as the finish line: "C x1,y1;x2,y2"
            Line checkpoint;
            ok = parseSegment(data, checkpoint);
            if (ok) checkpoints.push_back(checkpoint);
        }

        if (!ok) {
            return fail("Invalid track (line " + std::to_string(lineNumber) + ")");
        }
    }

    if (!hasStart) return fail("Invalid track: no start");
    if (!hasAngle) return fail("Invalid track: no direction");
    if (!hasFinish) return fail("Invalid track: no finish line");
    if (lines.empty()) return fail("Invalid track: no walls");

    return Track(lines, angleStart, start, finish, checkpoints);
}
