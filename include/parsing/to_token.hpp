#pragma once

#include <string>
#include <vector>
#include "error/nts_error.hpp"

namespace nts {

class ToToken {
public:
    ToToken(const std::string &filepath);
    std::vector<std::vector<std::string>> tokenize();

private:
    std::string _filepath;
    std::string remove_comment(const std::string &line);
    std::string trim(const std::string &str);
    std::vector<std::string> split(const std::string &line);
};

}
