#include "parsing/to_token.hpp"
#include <fstream>
#include <sstream>

namespace nts {

ToToken::ToToken(const std::string &filepath) : _filepath(filepath)
{
}

std::string ToToken::remove_comment(const std::string &line)
{
    std::size_t pos = line.find("#");
    if (pos == std::string::npos)
        return line;
    return line.substr(0, pos);
}

std::string ToToken::trim(const std::string &str)
{
    std::size_t start = str.find_first_not_of(" \t\n");
    if (start == std::string::npos)
        return "";
    std::size_t end = str.find_last_not_of(" \t\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> ToToken::split(const std::string &line)
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

std::vector<std::vector<std::string>> ToToken::tokenize()
{
    std::ifstream file(_filepath);
    if (!file.is_open()) {
        throw NtsError("Cannot open file: " + _filepath);
    }
    std::vector<std::vector<std::string>> lines;
    std::string line;

    while (std::getline(file, line)) {
        line = remove_comment(line);
        line = trim(line);
        if (line.empty())
            continue;
        std::vector<std::string> tokens = split(line);
        if (!tokens.empty())
            lines.push_back(tokens);
    }
    return lines;
}

}
