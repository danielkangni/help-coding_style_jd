#pragma once

#include <string>
#include <vector>
#include <utility>
#include "parsing/to_token.hpp"

namespace nts {

struct ChipsetInfo {
    std::string type;
    std::string name;
};

struct LinkInfo {
    std::string component1;
    std::size_t pin1;
    std::string component2;
    std::size_t pin2;
};

class Parser {
public:
    Parser(const std::string &filepath);
    void parse();
    const std::vector<ChipsetInfo> &get_chipsets() const;
    const std::vector<LinkInfo> &get_links() const;

private:
    enum class Section {
        NONE,
        CHIPSETS,
        LINKS
    };
    void parse_chipset(const std::vector<std::string> &line);
    void parse_link(const std::vector<std::string> &line);

    ToToken _tokenizer;
    std::vector<ChipsetInfo> _chipsets;
    std::vector<LinkInfo> _links;
};

}
