/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** main
*/

#include <iostream>
#include "plazza.hpp"

int main(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        std::cout << USAGE << std::endl;
        return 0;
    } else if (ac != 4) {
        std::cerr << USAGE << std::endl;
        return MY_EXIT_FAILURE;
    }
    return 0;
}
