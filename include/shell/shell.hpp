#pragma once

#include <string>
#include <csignal>
#include "circuit/circuit.hpp"
#include "circuit/tristate.hpp"

namespace nts {

class Shell {
    public:
        Shell(Circuit &circuit);
        void run();
        static void process_input_assignment(Circuit &circuit,
            const std::string &assignment);

    private:
        Circuit &_circuit;

        static nts::Tristate parse_value(const std::string &str);
        static void signal_handler(int signal);
        static volatile sig_atomic_t &loop_flag();
};

}
