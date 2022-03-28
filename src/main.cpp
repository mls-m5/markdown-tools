#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

auto helpStr = R"_(

usage:
-i                     where to put images
-p                     prefix to image filenames
)_";

void printHelp() {
    std::cout << helpStr << std::endl;
}

struct Settings {
    using path = std::filesystem::path;

    path markdownPath;
    path imagePath;
    std::string prefix;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string>(argv + 1, argv + argc);
        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);
            auto next = [&i, &args] { return args.at(++i); };

            if (arg == "-i") {
                imagePath = next();
            }
            if (arg == "-p") {
                prefix = next();
            }
            else {
                markdownPath = arg;
            }
        }

        if (imagePath.empty()) {
            printHelp();
            std::cerr << "missing image path\n";
            std::terminate();
        }
    }
};

int main(int argc, char **argv) {
    const auto settings = Settings{argc, argv};

    std::cout << settings.imagePath << std::endl;
}
