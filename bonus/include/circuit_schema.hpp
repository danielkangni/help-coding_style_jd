#pragma once

#include "parsing/parser.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

struct NodeInfo {
    std::string name;
    std::string type;
    sf::Vector2f pos;
    sf::Color color;
};

struct EdgeInfo {
    std::string from;
    std::size_t pin_from;
    std::string to;
    std::size_t pin_to;
};

class CircuitSchema {
public:
    CircuitSchema(const std::vector<nts::ChipsetInfo> &chipsets,
                  const std::vector<nts::LinkInfo>    &links,
                  sf::FloatRect area);

    const std::vector<NodeInfo> &nodes() const { return _nodes; }
    const std::vector<EdgeInfo> &edges() const { return _edges; }

    const NodeInfo *find_node(const std::string &name) const;

    void draw(sf::RenderWindow &win, const sf::Font &font,
              const std::unordered_map<std::string, std::string> &values) const;

private:
    std::vector<NodeInfo> _nodes;
    std::vector<EdgeInfo> _edges;
    sf::FloatRect         _area;

    static sf::Color color_for(const std::string &type);
};
