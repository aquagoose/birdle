module;

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

export module Birdle:Utils;

namespace bdl::Utils
{
    std::vector<std::string> LoadWordbank(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open wordbank file!");

        std::vector<std::string> words;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line.starts_with('#'))
                continue;

            words.push_back(line);
        }

        return words;
    }
}
