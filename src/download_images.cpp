#include "files.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

auto helpStr = R"_(

usage:
-i                     where to put images
-p                     prefix to image filenames
--dry-run              dont do anything, just print result
)_";

void printHelp() {
    std::cout << helpStr << std::endl;
}

struct Settings {
    using path = std::filesystem::path;

    path markdownPath;
    path imagePath;
    std::string prefix;
    bool shouldDryRun = false;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string>(argv + 1, argv + argc);
        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);
            auto next = [&i, &args] { return args.at(++i); };

            if (arg == "-i") {
                imagePath = next();
            }
            else if (arg == "-p") {
                prefix = next();
            }
            else if (arg == "--dry-run") {
                shouldDryRun = true;
            }
            else {
                markdownPath = getFile(arg);
            }
        }

        if (imagePath.empty()) {
            imagePath = getFile("z-images");
        }

        if (imagePath.empty()) {
            printHelp();
            std::cerr << "missing image path\n";
            std::terminate();
        }
    }
};

void download(std::string url, std::filesystem::path path) {
    std::cerr << "downloading to " << path << " from " << url << std::endl;

    auto ss = std::ostringstream{};
    ss << "wget " << url << " -O " << path;

    if (std::system(ss.str().c_str())) {
        std::cerr << "failed to download " << url << "\n";
        std::terminate();
    }
}

int main(int argc, char **argv) {
    const auto settings = Settings{argc, argv};

    auto file = std::ifstream{settings.markdownPath};

    if (!file) {
        std::cerr << "could not open markdown file " << settings.markdownPath
                  << "\n";
        std::terminate();
    }

    // Match link [...](...)
    auto re = std::regex{"!\\[(.*)\\]\\((.*)\\)"};

    auto filenameBase = settings.prefix + "-image";
    if (filenameBase.front() == '-') {
        filenameBase.erase(0, 1);
    }

    auto imageNum = size_t{0};

    auto os = std::ostringstream{};

    for (std::string line; std::getline(file, line);) {
        if (std::regex_match(line, re)) {
            auto count = size_t{0};
            auto replacementName = std::string{};
            for (auto it = std::sregex_iterator{line.begin(), line.end(), re};
                 it != std::sregex_iterator{};
                 ++it) {
                auto match = *it;
                ++count;

                // Dont know, maybe its not always pngs
                replacementName =
                    filenameBase + "-" + std::to_string(imageNum) + ".png";

                if (!settings.shouldDryRun) {
                    download(match[2], settings.imagePath / replacementName);
                }
            }

            if (count > 1) {
                std::cerr << "line " << line << ":\n";
                std::cerr << "multiple images per line is not supported\n";
                std::terminate();
            }

            os << std::regex_replace(line, re, "![[" + replacementName + "]]")
               << "\n";
            ++imageNum;
        }

        else {
            os << line << "\n";
        }
    }

    file.close();

    if (!settings.shouldDryRun) {
        auto outfile = std::ofstream{settings.markdownPath};

        outfile << os.str();
    }
    else {
        std::cout << os.str();
    }
}
