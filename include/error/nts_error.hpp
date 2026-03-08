#pragma once

#include <exception>
#include <string>

namespace nts {

class NtsError : public std::exception {
public:
    NtsError(const std::string &message) : _message(message) {}
    const char *what() const noexcept override {
        return _message.c_str();
    }

private:
    std::string _message;
};

}
