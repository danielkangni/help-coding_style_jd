#pragma once

#include "circuit/i_component.hpp"
#include <string>
#include <map>

namespace nts {

class AComponent : public IComponent {
public:
    AComponent(const std::string &name) : _name(name) {}
    const std::string &get_name() const override {
        return _name;
    }
    void simulate() override {}

protected:
    std::string _name;
    std::map<std::size_t, std::pair<IComponent*, std::size_t>> _links;
};

}
