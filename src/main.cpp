/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** main
*/

#include <iostream>
#include "plazza.hpp"
#include "Reception.hpp"

static bool args_are_correct(int ac, char **av);

int main(int ac, char **av) {
    plz::Reception reception;
    std::string orders;

    if (ac == 2 && std::string(av[1]) == "-h") {
        std::cout << USAGE << std::endl;
        return 0;
    } else if (!args_are_correct(ac, av)) {
        std::cerr << USAGE << std::endl;
        return MY_EXIT_FAILURE;
    }
    while (std::cin) {
        getline(std::cin, orders);
        reception.placeOrders(orders);
    }
    return 0;
}

static bool args_are_correct(int ac, char **av) {
    return (
        ac == 4
        && isPositiveNumber<float>(av[1])
        && isPositiveNumber<int>(av[2])
        && isPositiveNumber<int>(av[3]));
}
