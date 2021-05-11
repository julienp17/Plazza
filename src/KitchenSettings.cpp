/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** KitchenSettings
*/

#include "KitchenSettings.hpp"

namespace plz {
KitchenSettings::KitchenSettings(void) {
    this->cookingMultiplier = 1.0f;
    this->nbCooks = 5;
    this->startNbIngredients = 5;
    this->inactiveTime = std::chrono::milliseconds(5000);
    this->restockTime = std::chrono::milliseconds(2000);
    this->restockNb = 1;
}
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::KitchenSettings &sett) {
    out << "Kitchen settings:" << std::endl
        << "Cooking multiplier = " << sett.cookingMultiplier << std::endl
        << "Number of cooks = " << sett.nbCooks << std::endl
        << "Restocking time = " << sett.restockTime.count() << "ms";
    return out;
}
