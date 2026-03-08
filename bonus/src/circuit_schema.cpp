#include "circuit_schema.hpp"
#include <cmath>
#include <algorithm>

static const float NODE_W  = 100.f;
static const float NODE_H  =  40.f;
static const float MARGIN  =  20.f;

sf::Color CircuitSchema::color_for(const std::string &type)
{
    if (type == "input")  return sf::Color(70, 130, 180);
    if (type == "clock")  return sf::Color(128, 0, 128);
    if (type == "output") return sf::Color(210, 105, 30);
    if (type == "true" || type == "false") return sf::Color(80, 80, 80);
    return sf::Color(34, 100, 34);
}

CircuitSchema::CircuitSchema(const std::vector<nts::ChipsetInfo> &chipsets,
                             const std::vector<nts::LinkInfo>    &links,
                             sf::FloatRect area)
    : _area(area)
{
    std::vector<const nts::ChipsetInfo *> inputs, outputs, logic;

    for (const auto &c : chipsets) {
        if (c.type == "input" || c.type == "clock")
            inputs.push_back(&c);
        else if (c.type == "output")
            outputs.push_back(&c);
        else
            logic.push_back(&c);
    }

    auto place_column = [&](const std::vector<const nts::ChipsetInfo *> &col,
                             float x_center)
    {
        float total_h = col.size() * (NODE_H + MARGIN) - MARGIN;
        float y_start = area.top + (area.height - total_h) / 2.f;
        for (std::size_t i = 0; i < col.size(); ++i) {
            NodeInfo n;
            n.name  = col[i]->name;
            n.type  = col[i]->type;
            n.pos   = { x_center - NODE_W / 2.f,
                        y_start + i * (NODE_H + MARGIN) };
            n.color = color_for(col[i]->type);
            _nodes.push_back(n);
        }
    };

    float col_in  = area.left + NODE_W / 2.f + MARGIN;
    float col_out = area.left + area.width - NODE_W / 2.f - MARGIN;

    float mid_left  = col_in  + NODE_W + MARGIN * 2;
    float mid_right = col_out - NODE_W - MARGIN * 2;
    float mid_width = mid_right - mid_left;

    int cols = std::max(1, static_cast<int>(std::ceil(std::sqrt((float)logic.size()))));
    int rows = std::max(1, (int)std::ceil((float)logic.size() / cols));

    float step_x = (logic.size() > 1 && cols > 1)
                   ? mid_width / (cols - 1)
                   : mid_width / 2.f;
    float step_y = (rows > 1)
                   ? (area.height - MARGIN * 2 - NODE_H) / (rows - 1)
                   : (area.height - NODE_H) / 2.f;

    for (std::size_t i = 0; i < logic.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        NodeInfo n;
        n.name  = logic[i]->name;
        n.type  = logic[i]->type;
        n.pos   = { mid_left + col * step_x - NODE_W / 2.f,
                    area.top + MARGIN + row * step_y };
        n.color = color_for(logic[i]->type);
        _nodes.push_back(n);
    }

    place_column(inputs,  col_in);
    place_column(outputs, col_out);

    for (const auto &lnk : links) {
        _edges.push_back({ lnk.component1, lnk.pin1,
                           lnk.component2, lnk.pin2 });
    }
}

const NodeInfo *CircuitSchema::find_node(const std::string &name) const
{
    for (const auto &n : _nodes)
        if (n.name == name) return &n;
    return nullptr;
}

void CircuitSchema::draw(sf::RenderWindow &win, const sf::Font &font,
                         const std::unordered_map<std::string, std::string> &values) const
{
    for (const auto &e : _edges) {
        const NodeInfo *a = find_node(e.from);
        const NodeInfo *b = find_node(e.to);
        if (!a || !b) continue;

        sf::Vector2f pa = { a->pos.x + NODE_W, a->pos.y + NODE_H / 2.f };
        sf::Vector2f pb = { b->pos.x,           b->pos.y + NODE_H / 2.f };

        sf::Vertex line[2] = {
            sf::Vertex(pa, sf::Color(200, 200, 200)),
            sf::Vertex(pb, sf::Color(200, 200, 200))
        };
        win.draw(line, 2, sf::Lines);

        auto draw_pin_label = [&](sf::Vector2f pos, std::size_t pin) {
            sf::Text t;
            t.setFont(font);
            t.setString(std::to_string(pin));
            t.setCharacterSize(10);
            t.setFillColor(sf::Color(180, 180, 100));
            t.setPosition(pos.x + 2, pos.y - 8);
            win.draw(t);
        };
        draw_pin_label(pa, e.pin_from);
        draw_pin_label(pb, e.pin_to);
    }

    for (const auto &n : _nodes) {
        sf::RectangleShape rect({ NODE_W, NODE_H });
        rect.setPosition(n.pos);
        rect.setFillColor(n.color);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1.f);
        win.draw(rect);

        sf::Text name_text;
        name_text.setFont(font);
        name_text.setString(n.name);
        name_text.setCharacterSize(13);
        name_text.setFillColor(sf::Color::White);
        name_text.setStyle(sf::Text::Bold);
        auto nb = name_text.getLocalBounds();
        name_text.setPosition(n.pos.x + (NODE_W - nb.width) / 2.f,
                              n.pos.y + 4.f);
        win.draw(name_text);

        sf::Text type_text;
        type_text.setFont(font);
        type_text.setString(n.type);
        type_text.setCharacterSize(10);
        type_text.setFillColor(sf::Color(220, 220, 220));
        auto tb = type_text.getLocalBounds();
        type_text.setPosition(n.pos.x + (NODE_W - tb.width) / 2.f,
                              n.pos.y + NODE_H - 16.f);
        win.draw(type_text);

        auto it = values.find(n.name);
        if (it != values.end()) {
            sf::Text val_text;
            val_text.setFont(font);
            val_text.setString(it->second);
            val_text.setCharacterSize(14);
            sf::Color vc = sf::Color::White;
            if (it->second == "1") vc = sf::Color(0, 230, 0);
            else if (it->second == "0") vc = sf::Color(230, 80, 80);
            val_text.setFillColor(vc);
            val_text.setStyle(sf::Text::Bold);
            auto vb = val_text.getLocalBounds();
            val_text.setPosition(n.pos.x + NODE_W + 4.f,
                                 n.pos.y + (NODE_H - vb.height) / 2.f - 2.f);
            win.draw(val_text);
        }
    }
}
