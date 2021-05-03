/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** main
*/

#include <iostream>
#include "plazza.hpp"
#include "Reception.hpp"
#include "Kitchen.hpp"

static bool args_are_correct(int ac, char **av);
static void runReception(plz::Reception &reception);

int main(int ac, char **av) {
    plz::Reception reception;
    plz::KitchenSettings settings;

    if (ac == 2 && std::string(av[1]) == "-h")
        exitUsage(std::cout, 0);
    else if (!args_are_correct(ac, av))
        exitUsage(std::cerr, MY_EXIT_FAILURE);
    settings.cookingMultiplier = getNumber<float>(av[1]);
    settings.nbCooks = getNumber<size_t>(av[2]);
    settings.restockTime = std::chrono::milliseconds(getNumber<size_t>(av[3]));
    runReception(reception);
    return 0;
}

static bool args_are_correct(int ac, char **av) {
    return (
        ac == 4
        && isPositiveNumber<float>(av[1])
        && isPositiveNumber<size_t>(av[2])
        && isPositiveNumber<size_t>(av[3]));
}

static void runReception(plz::Reception &reception) {
    std::string orders;

    while (std::cin) {
        getline(std::cin, orders);
        reception.placeOrders(orders);
    }
}
