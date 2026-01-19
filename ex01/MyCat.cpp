/*
** EPITECH PROJECT, 2025
** algo_step1
** File description:
** step1
*/

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 84;
    }
    for (int i = 1; i < argc; i++) {
        std::string nomdufichier = argv[i];
        std::ifstream monFichier(nomdufichier);
        std::string ligne;
        if (monFichier.is_open()) {
            while (getline(monFichier, ligne)) {
                std::cout << ligne << std::endl;
            }
            monFichier.close();
        } else {
            std::cout << "MyCat : file : No such file or directory" << std::endl;
            return 84;
        }
    }
    return 0;
}