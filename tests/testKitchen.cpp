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
    std::unordered_map<std::string, size_t> stock;
    plz::Kitchen kitchen;
    plz::KitchenSettings settings = kitchen.getSettings();

    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, settings.startNbIngredients);
}

TEST(Kitchen, restockingDefaultValue) {
    std::unordered_map<std::string, size_t> stock;
    plz::Kitchen kitchen;
    plz::KitchenSettings settings = kitchen.getSettings();

    kitchen.restock();
    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, settings.startNbIngredients + settings.restockNb);
}

TEST(Kitchen, restockingChangedValue) {
    std::unordered_map<std::string, size_t> stock;
    plz::Kitchen kitchen;
    plz::KitchenSettings settings = kitchen.getSettings();

    settings.restockNb = 2;
    kitchen.setSettings(settings);
    kitchen.restock();
    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, settings.startNbIngredients + 2);
}
