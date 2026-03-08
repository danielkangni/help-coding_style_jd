#include <iostream>
#include <string>
#include "parsing/parser.hpp"
#include "circuit/circuit.hpp"
#include "shell/shell.hpp"
#include "error/nts_error.hpp"

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: ./nanotekspice file.nts [input=value ...]" << std::endl;
        return 84;
    }

    try {
        nts::Parser parser(argv[1]);
        parser.parse();

        nts::Circuit circuit(parser.get_chipsets(), parser.get_links());
        nts::Shell shell(circuit);

        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];
            if (arg.find('=') != std::string::npos) {
                nts::Shell::process_input_assignment(circuit, arg);
            } else {
                std::cerr << "Warning: ignoring invalid argument: " << arg << std::endl;
            }
        }

        shell.run();

    } catch (const nts::NtsError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
