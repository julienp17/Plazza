/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** testKitchen
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <vector>
#include "Kitchen.hpp"
#include "Error.hpp"

TEST(Kitchen, stockUponCreation) {
    plz::KitchenSettings settings;
    plz::Kitchen kitchen;
    std::unordered_map<std::string, size_t> stock;

    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, settings.startNbIngredients);
}

TEST(Kitchen, restocking) {
    plz::KitchenSettings settings;
    plz::Kitchen kitchen;
    std::unordered_map<std::string, size_t> stock;

    kitchen.restock();
    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, settings.startNbIngredients + 1);
}
