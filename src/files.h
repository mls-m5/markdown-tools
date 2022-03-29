#pragma once

#include <filesystem>
#include <string>

inline std::filesystem::path getFile(std::string name) {
    if (std::filesystem::exists(name)) {
        return name;
    }

    for (auto &it : std::filesystem::recursive_directory_iterator{
             std::filesystem::current_path()}) {
        if (it.path().stem() == name) {
            return it.path();
        }
    }

    return {};
}
