#include "files.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

namespace {

bool isExtraBlankLine(std::string_view line) {
    if (line.empty()) {
        return false;
    }

    for (auto c : line) {
        if (!isspace(c)) {
            return false;
        }
    }

    return true;
}

} // namespace

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "no file specified\n";
    }
    if (argc > 2) {
        std::cerr << "to many arguments\n";
    }

    auto path = getFile(argv[1]);

    if (path.empty()) {
        std::cerr << "could not find file " << argv[1] << "\n";
        std::terminate();
    }

    auto os = std::ostringstream{};

    {
        auto file = std::ifstream{path};
        if (!file) {
            std::cout << "could not open " << path << "\n";
            std::terminate();
        }
        bool wasBlankLine = false;
        for (std::string line; std::getline(file, line);) {
            if (wasBlankLine && line.empty()) {
                continue;
            }
            wasBlankLine = false;

            if (isExtraBlankLine(line)) {
                wasBlankLine = true;
                continue;
            }

            os << line << "\n";
        }
    }

    //    std::cout << os.str();
    std::ofstream{path} << os.str();

    return 0;
}
