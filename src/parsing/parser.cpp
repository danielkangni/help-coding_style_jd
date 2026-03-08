#include "parsing/parser.hpp"
#include "error/nts_error.hpp"
#include <iostream>

namespace nts {

Parser::Parser(const std::string &filepath) : _tokenizer(filepath)
{
}

void Parser::parse()
{
    std::vector<std::vector<std::string>> tokens = _tokenizer.tokenize();
    Section section = Section::NONE;
    bool has_chipsets = false;
    bool has_links = false;

    for (const auto &line : tokens) {
        if (line[0] == ".chipsets:") {
            if (has_chipsets)
                throw NtsError("Duplicate .chipsets: section");
            section = Section::CHIPSETS;
            has_chipsets = true;
            continue;
        }
        if (line[0] == ".links:") {
            if (has_links)
                throw NtsError("Duplicate .links: section");
            section = Section::LINKS;
            has_links = true;
            continue;
        }
        if (section == Section::NONE)
            throw NtsError("Content outside of section");
        if (section == Section::CHIPSETS)
            parse_chipset(line);
        else if (section == Section::LINKS)
            parse_link(line);
    }
    if (!has_chipsets)
        throw NtsError("Missing .chipsets: section");
    if (!has_links)
        throw NtsError("Missing .links: section");
}

void Parser::parse_chipset(const std::vector<std::string> &line)
{
    if (line.size() != 2)
        throw NtsError("Invalid chipset definition (expected: type name)");
    std::string type = line[0];
    std::string name = line[1];
    for (const auto &chip : _chipsets) {
        if (chip.name == name)
            throw NtsError("Duplicate chipset name: " + name);
    }
    _chipsets.push_back({type, name});
}

void Parser::parse_link(const std::vector<std::string> &line)
{
    if (line.size() != 2)
        throw NtsError("Invalid link definition (expected: comp:pin comp:pin)");
    std::string left = line[0];
    std::string right = line[1];

    std::size_t sep1 = left.find(':');
    std::size_t sep2 = right.find(':');

    if (sep1 == std::string::npos || sep2 == std::string::npos)
        throw NtsError("Invalid link format (missing ':')");

    std::string comp1 = left.substr(0, sep1);
    std::string pin_str1 = left.substr(sep1 + 1);
    std::string comp2 = right.substr(0, sep2);
    std::string pin_str2 = right.substr(sep2 + 1);

    if (comp1.empty() || pin_str1.empty() || comp2.empty() || pin_str2.empty())
        throw NtsError("Invalid link format (empty component or pin)");

    std::size_t pin1 = 0;
    std::size_t pin2 = 0;
    try {
        pin1 = std::stoul(pin_str1);
        pin2 = std::stoul(pin_str2);
    } catch (...) {
        throw NtsError("Invalid pin number in link");
    }
    if (pin1 == 0 || pin2 == 0)
        throw NtsError("Pin numbers must be > 0");
    _links.push_back({comp1, pin1, comp2, pin2});
}

const std::vector<ChipsetInfo> &Parser::get_chipsets() const
{
    return _chipsets;
}

const std::vector<LinkInfo> &Parser::get_links() const
{
    return _links;
}

}
