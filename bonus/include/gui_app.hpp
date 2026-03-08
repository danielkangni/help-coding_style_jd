#pragma once

#include "circuit_schema.hpp"
#include "process.hpp"
#include "parsing/parser.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

struct InputRow {
    std::string name;
    sf::FloatRect btn_1;
    sf::FloatRect btn_0;
    sf::FloatRect btn_u;
};

class GuiApp {
public:
    GuiApp(const std::string &nts_file,
           const std::vector<nts::ChipsetInfo> &chipsets,
           const std::vector<nts::LinkInfo>    &links);

    void run();

private:
    void handle_events();
    void handle_click(sf::Vector2f pos);
    void update_output();
    void draw_panel();
    void draw_tick();

    sf::RenderWindow  _win;
    sf::Font          _font;
    CircuitSchema     _schema;
    Process           _proc;

    std::vector<InputRow>                        _input_rows;
    std::vector<std::string>                     _output_names;
    std::unordered_map<std::string, std::string> _values;

    sf::FloatRect _btn_simulate;
    sf::FloatRect _btn_loop;

    bool        _looping = false;
    std::string _raw_output;
    std::string _tick;

    sf::Clock _loop_clock;
    float     _loop_interval = 0.1f;

    static constexpr float WIN_W    = 1280.f;
    static constexpr float WIN_H    =  800.f;
    static constexpr float PANEL_X  =  950.f;
    static constexpr float SCHEMA_W =  PANEL_X;
    static constexpr float BTN_H    =   28.f;
    static constexpr float BTN_W    =   36.f;
    static constexpr float ROW_H    =   38.f;
};
