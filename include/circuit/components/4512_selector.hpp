#pragma once
#include "circuit/i_component.hpp"
#include "circuit/components/logic_ops.hpp"
#include <string>
#include <map>
#include <functional>

namespace nts {
    class Selector4512 : public IComponent {
    public:
        Selector4512(const std::string &name);
         Tristate compute(std::size_t pin) override;
        void set_link(std::size_t pin, IComponent &other, std::size_t other_pin) override;
        void simulate() override {}
        const std::string &get_name() const override { return _name; }
    private:
        std::string _name;
        std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
    };
}