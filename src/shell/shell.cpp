#include <iostream>
#include <csignal>
#include "shell/shell.hpp"
#include "error/nts_error.hpp"

namespace nts {

Shell::Shell(Circuit &circuit) : _circuit(circuit)
{
}

volatile sig_atomic_t &Shell::loop_flag()
{
    static volatile sig_atomic_t running = 0;
    return running;
}

void Shell::signal_handler(int signal)
{
    if (signal == SIGINT) {
        loop_flag() = 0;
    }
}

nts::Tristate Shell::parse_value(const std::string &str)
{
    if (str == "0")
        return Tristate::False;
    if (str == "1")
        return Tristate::True;
    if (str == "U")
        return Tristate::Undefined;
    throw NtsError("Invalid value: " + str + " (must be 0, 1, or U)");
}

void Shell::process_input_assignment(Circuit &circuit,
    const std::string &assignment)
{
    std::size_t pos = assignment.find('=');
    if (pos == std::string::npos)
        throw NtsError("Invalid input assignment: " + assignment);

    std::string name = assignment.substr(0, pos);
    std::string value_str = assignment.substr(pos + 1);

    Tristate value = parse_value(value_str);
    circuit.set_input(name, value);
}

void Shell::run()
{
    std::string line;
    std::signal(SIGINT, signal_handler);

    while (true) {
        std::cout << "> " << std::flush;

        if (!std::getline(std::cin, line)) {
            break;
        }

        std::size_t start = line.find_first_not_of(" \t");
        std::size_t end = line.find_last_not_of(" \t");
        if (start == std::string::npos) {
            continue;
        }
        line = line.substr(start, end - start + 1);

        if (line.empty())
            continue;

        try {
            if (line == "exit") {
                break;
            } else if (line == "display") {
                _circuit.display();
            } else if (line == "simulate") {
                _circuit.simulate();
            } else if (line == "loop") {
                loop_flag() = 1;
                while (loop_flag()) {
                    _circuit.simulate();
                    _circuit.display();
                }
            } else if (line.find('=') != std::string::npos) {
                process_input_assignment(_circuit, line);
            } else {
                std::cerr << "Unknown command: " << line << std::endl;
            }
        } catch (const NtsError &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

}
