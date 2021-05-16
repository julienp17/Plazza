/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** KitchenSettings
*/

#ifndef INC_KITCHENSETTINGS_HPP_
#define INC_KITCHENSETTINGS_HPP_

#include <iostream>
#include <chrono>

namespace plz {
/**
 * @brief Settings for a kitchen
 */
struct KitchenSettings {
    /**
     * @brief Construct a new Kitchen Settings object with default values
     */
    KitchenSettings(void);

    /**
     * @brief Multiplier for the cooking time of the pizzas
     */
    float cookingMultiplier;

    /**
     * @brief Number of cooks in the kitchen
     */
    size_t nbCooks;

    /**
     * @brief The starting stock of each ingredient
     */
    size_t startNbIngredients;

    /**
     * @brief Time for a kitchen stock to replace ingredients
     */
    std::chrono::milliseconds restockTime;

    /**
     * @brief Maximum time for a kitchen to be inactive before it closes
     */
    std::chrono::milliseconds inactiveTime;

    /**
     * @brief The number to restock for each ingredient
     */
    size_t restockNb;
};
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::KitchenSettings &settin);

#endif  // INC_KITCHENSETTINGS_HPP_
