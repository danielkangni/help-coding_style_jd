#include "gui_app.hpp"
#include <sstream>
#include <algorithm>
#include <cstring>

static bool contains(sf::FloatRect r, sf::Vector2f p)
{
    return r.contains(p);
}

static void draw_button(sf::RenderWindow &win, const sf::Font &font,
                        sf::FloatRect r, const std::string &label,
                        sf::Color bg, sf::Color fg = sf::Color::White)
{
    sf::RectangleShape box({ r.width, r.height });
    box.setPosition(r.left, r.top);
    box.setFillColor(bg);
    box.setOutlineColor(sf::Color(200, 200, 200));
    box.setOutlineThickness(1.f);
    win.draw(box);

    sf::Text t;
    t.setFont(font);
    t.setString(label);
    t.setCharacterSize(13);
    t.setFillColor(fg);
    auto tb = t.getLocalBounds();
    t.setPosition(r.left + (r.width  - tb.width)  / 2.f,
                  r.top  + (r.height - tb.height) / 2.f - 2.f);
    win.draw(t);
}

static std::string trim(const std::string &s)
{
    std::size_t a = s.find_first_not_of("> \t\r\n");
    if (a == std::string::npos) return "";
    std::size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static void parse_display(const std::string &raw,
                           std::unordered_map<std::string, std::string> &values,
                           std::string &tick)
{
    std::istringstream ss(raw);
    std::string line;
    while (std::getline(ss, line)) {
        auto colon = line.rfind(':');
        if (colon == std::string::npos) continue;
        std::string key = trim(line.substr(0, colon));
        std::string val = trim(line.substr(colon + 1));
        if (val.empty() || key.empty()) continue;
        if (key == "tick") {
            tick = val;
            continue;
        }
        if (val != "0" && val != "1" && val != "U") continue;
        values[key] = val;
    }
}

GuiApp::GuiApp(const std::string &nts_file,
               const std::vector<nts::ChipsetInfo> &chipsets,
               const std::vector<nts::LinkInfo>    &links)
    : _win(sf::VideoMode((unsigned)WIN_W, (unsigned)WIN_H),
           "NanoTekSpice — " + nts_file,
           sf::Style::Close)
    , _schema(chipsets, links, sf::FloatRect(0, 0, SCHEMA_W, WIN_H))
    , _proc("stdbuf", { "-o0", "./nanotekspice", nts_file })
{
    _win.setFramerateLimit(60);

    if (!_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"))
        _font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");

    float y = 60.f;
    for (const auto &c : chipsets) {
        if (c.type == "input" || c.type == "clock") {
            InputRow row;
            row.name  = c.name;
            float x   = PANEL_X + 8.f;
            row.btn_1 = { x + 130.f, y, BTN_W, BTN_H };
            row.btn_0 = { x + 172.f, y, BTN_W, BTN_H };
            row.btn_u = { x + 214.f, y, BTN_W, BTN_H };
            _input_rows.push_back(row);
            _values[c.name] = "U";
            y += ROW_H;
        }
        if (c.type == "output") {
            _output_names.push_back(c.name);
            _values[c.name] = "U";
        }
    }

    float btn_y = WIN_H - 60.f;
    _btn_simulate = { PANEL_X + 8.f,         btn_y, 148.f, 34.f };
    _btn_loop     = { PANEL_X + 8.f + 158.f, btn_y, 148.f, 34.f };

    _proc.send("display");
}

void GuiApp::run()
{
    while (_win.isOpen()) {
        handle_events();

        if (_looping && _loop_clock.getElapsedTime().asSeconds() >= _loop_interval) {
            _loop_clock.restart();
            _proc.send("simulate");
            _proc.send("display");
        }

        update_output();

        _win.clear(sf::Color(30, 30, 30));

        sf::RectangleShape bg({ SCHEMA_W, WIN_H });
        bg.setFillColor(sf::Color(20, 50, 20));
        _win.draw(bg);

        for (int gx = 20; gx < (int)SCHEMA_W; gx += 30)
            for (int gy = 20; gy < (int)WIN_H; gy += 30) {
                sf::CircleShape dot(1.5f);
                dot.setFillColor(sf::Color(40, 80, 40));
                dot.setPosition((float)gx, (float)gy);
                _win.draw(dot);
            }

        _schema.draw(_win, _font, _values);
        draw_panel();

        _win.display();
    }
}

void GuiApp::handle_events()
{
    sf::Event ev;
    while (_win.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed)
            _win.close();
        if (ev.type == sf::Event::KeyPressed &&
            ev.key.code == sf::Keyboard::Escape)
            _win.close();
        if (ev.type == sf::Event::MouseButtonPressed &&
            ev.mouseButton.button == sf::Mouse::Left)
            handle_click({ (float)ev.mouseButton.x, (float)ev.mouseButton.y });
    }
}

void GuiApp::handle_click(sf::Vector2f pos)
{
    if (contains(_btn_simulate, pos)) {
        _proc.send("simulate");
        _proc.send("display");
        return;
    }
    if (contains(_btn_loop, pos)) {
        _looping = !_looping;
        if (!_looping) _proc.send("display");
        return;
    }
    for (const auto &row : _input_rows) {
        if (contains(row.btn_1, pos)) {
            _proc.send(row.name + "=1");
            _values[row.name] = "1";
        } else if (contains(row.btn_0, pos)) {
            _proc.send(row.name + "=0");
            _values[row.name] = "0";
        } else if (contains(row.btn_u, pos)) {
            _proc.send(row.name + "=U");
            _values[row.name] = "U";
        }
    }
}

void GuiApp::update_output()
{
    std::string chunk = _proc.read_available();
    if (!chunk.empty()) {
        _raw_output += chunk;
        parse_display(_raw_output, _values, _tick);
        if (_raw_output.size() > 4096)
            _raw_output = _raw_output.substr(_raw_output.size() - 4096);
    }
}

void GuiApp::draw_tick()
{
    sf::Text t;
    t.setFont(_font);
    t.setString("tick: " + (_tick.empty() ? "0" : _tick));
    t.setCharacterSize(15);
    t.setFillColor(sf::Color(255, 200, 50));
    t.setStyle(sf::Text::Bold);
    auto tb = t.getLocalBounds();
    t.setPosition(WIN_W - tb.width - 12.f, 10.f);
    _win.draw(t);
}

void GuiApp::draw_panel()
{
    sf::RectangleShape panel({ WIN_W - PANEL_X, WIN_H });
    panel.setPosition(PANEL_X, 0);
    panel.setFillColor(sf::Color(40, 40, 40));
    panel.setOutlineColor(sf::Color(80, 80, 80));
    panel.setOutlineThickness(1.f);
    _win.draw(panel);

    float x = PANEL_X + 8.f;

    sf::Text title;
    title.setFont(_font);
    title.setString("Controls");
    title.setCharacterSize(16);
    title.setFillColor(sf::Color(200, 200, 200));
    title.setStyle(sf::Text::Bold);
    title.setPosition(x, 10.f);
    _win.draw(title);

    draw_tick();

    sf::RectangleShape sep({ WIN_W - PANEL_X - 16.f, 1.f });
    sep.setPosition(x, 36.f);
    sep.setFillColor(sf::Color(80, 80, 80));
    _win.draw(sep);

    sf::Text hdr;
    hdr.setFont(_font);
    hdr.setString("Inputs:");
    hdr.setCharacterSize(12);
    hdr.setFillColor(sf::Color(150, 150, 150));
    hdr.setPosition(x, 42.f);
    _win.draw(hdr);

    for (const auto &row : _input_rows) {
        sf::Text nm;
        nm.setFont(_font);
        nm.setString(row.name);
        nm.setCharacterSize(13);
        nm.setFillColor(sf::Color::White);
        nm.setPosition(x, row.btn_1.top + 6.f);
        _win.draw(nm);

        auto vit = _values.find(row.name);
        std::string cur = (vit != _values.end()) ? vit->second : "U";
        sf::Color vc = (cur == "1") ? sf::Color(0, 200, 0)
                     : (cur == "0") ? sf::Color(200, 60, 60)
                     : sf::Color(150, 150, 150);
        sf::CircleShape dot(5.f);
        dot.setFillColor(vc);
        dot.setPosition(x + 100.f, row.btn_1.top + 9.f);
        _win.draw(dot);

        draw_button(_win, _font, row.btn_1, "1",
                    (cur == "1") ? sf::Color(0, 150, 0) : sf::Color(60, 90, 60));
        draw_button(_win, _font, row.btn_0, "0",
                    (cur == "0") ? sf::Color(180, 40, 40) : sf::Color(90, 60, 60));
        draw_button(_win, _font, row.btn_u, "U",
                    (cur == "U") ? sf::Color(100, 100, 100) : sf::Color(60, 60, 60));
    }

    float oy = 60.f + _input_rows.size() * ROW_H + 20.f;
    sf::RectangleShape sep2({ WIN_W - PANEL_X - 16.f, 1.f });
    sep2.setPosition(x, oy - 8.f);
    sep2.setFillColor(sf::Color(80, 80, 80));
    _win.draw(sep2);

    sf::Text ohdr;
    ohdr.setFont(_font);
    ohdr.setString("Outputs:");
    ohdr.setCharacterSize(12);
    ohdr.setFillColor(sf::Color(150, 150, 150));
    ohdr.setPosition(x, oy);
    _win.draw(ohdr);
    oy += 20.f;

    for (const auto &name : _output_names) {
        auto vit = _values.find(name);
        std::string val = (vit != _values.end()) ? vit->second : "U";
        sf::Color vc = (val == "1") ? sf::Color(0, 230, 0)
                     : (val == "0") ? sf::Color(230, 60, 60)
                     : sf::Color(160, 160, 160);

        sf::Text ot;
        ot.setFont(_font);
        ot.setString(name + ":  " + val);
        ot.setCharacterSize(14);
        ot.setFillColor(vc);
        ot.setStyle(sf::Text::Bold);
        ot.setPosition(x, oy);
        _win.draw(ot);
        oy += ROW_H;
    }

    draw_button(_win, _font, _btn_simulate, "SIMULATE", sf::Color(50, 100, 160));
    draw_button(_win, _font, _btn_loop,
                _looping ? "LOOP (ON)" : "LOOP (OFF)",
                _looping ? sf::Color(160, 80, 0) : sf::Color(60, 60, 60));
}
