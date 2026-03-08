#include "gui_app.hpp"
#include "parsing/parser.hpp"
#include "error/nts_error.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: nanotekspice-gui <file.nts>\n";
        return 84;
    }

    try {
        nts::Parser parser(argv[1]);
        parser.parse();

        GuiApp app(argv[1], parser.get_chipsets(), parser.get_links());
        app.run();
    } catch (const nts::NtsError &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 84;
    }
    return 0;
}
