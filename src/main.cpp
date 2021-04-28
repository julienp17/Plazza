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
static void runReception(const plz::ReceptionPtr reception);

int main(int ac, char **av) {
    plz::ReceptionPtr reception = nullptr;
    float cookingMultiplier = 0.0f;
    size_t nbCooks = 0;
    plz::milliseconds_t stockTime;

    if (ac == 2 && std::string(av[1]) == "-h")
        exitUsage(std::cout, 0);
    else if (!args_are_correct(ac, av))
        exitUsage(std::cerr, MY_EXIT_FAILURE);
    cookingMultiplier = getNumber<float>(av[1]);
    nbCooks = getNumber<size_t>(av[2]);
    stockTime = (plz::milliseconds_t)getNumber<size_t>(av[3]);
    reception = plz::ReceptionPtr(new plz::Reception(cookingMultiplier, nbCooks,
                                                    stockTime));
    runReception(reception);
    return 0;
}

static bool args_are_correct(int ac, char **av) {
    return (
        ac == 4
        && isPositiveNumber<float>(av[1])
        && isPositiveNumber<int>(av[2])
        && isPositiveNumber<int>(av[3]));
}

static void runReception(const plz::ReceptionPtr reception) {
    std::string orders;

    while (std::cin) {
        getline(std::cin, orders);
        reception->placeOrders(orders);
    }
}
