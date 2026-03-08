#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "parsing/parser.hpp"
#include "circuit/tristate.hpp"
#include "circuit/i_component.hpp"

namespace nts {

    class Circuit {
        private:
        void create_components(const std::vector<ChipsetInfo> &chipsets);

        void create_links(const std::vector<LinkInfo> &links);

        IComponent *get_component(const std::string &name);

        std::map<std::string, std::unique_ptr<IComponent>> _components;
        std::vector<std::string> _input_names;
        std::vector<std::string> _output_names;
        std::size_t _tick = 0;
        void display_pin(const std::string &name, Tristate value) const;

        public:
        Circuit(const std::vector<ChipsetInfo> &chipsets,
        const std::vector<LinkInfo> &links);

        void set_input(const std::string &name, Tristate value);

        void simulate();

        void display() const;

        std::size_t get_tick() {
            return _tick;
        }

    };
}
