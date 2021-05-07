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

static bool argsAreCorrect(int ac, char **av);
plz::KitchenSettings getSettings(char **av);

int main(int ac, char **av) {
    plz::Reception reception;

    if (ac == 2 && std::string(av[1]) == "-h")
        returnUsage(std::cout, 0);
    else if (!argsAreCorrect(ac, av))
        returnUsage(std::cerr, MY_EXIT_FAILURE);
    reception.setKitchenSettings(getSettings(av));
    reception.run();
    return 0;
}

static bool argsAreCorrect(int ac, char **av) {
    return (
        ac == 4
        && isPositiveNumber<float>(av[1])
        && isPositiveNumber<size_t>(av[2])
        && isPositiveNumber<size_t>(av[3]));
}

plz::KitchenSettings getSettings(char **av) {
    plz::KitchenSettings settings;

    settings.cookingMultiplier = getNumber<float>(av[1]);
    settings.nbCooks = getNumber<size_t>(av[2]);
    settings.restockTime = std::chrono::milliseconds(getNumber<size_t>(av[3]));
    return settings;
}
