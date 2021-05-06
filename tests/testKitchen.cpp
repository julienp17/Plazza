/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** testKitchen
*/

#include <gtest/gtest.h>
#include <algorithm>
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

TEST(Kitchen, canAddPizzasOneCook) {
    plz::KitchenSettings settings;
    plz::Kitchen kitchen;

    settings.nbCooks = 1;
    kitchen.setSettings(settings);
    ASSERT_TRUE(kitchen.canAddPizzas(1));
    kitchen.addPizza(std::make_shared<plz::Pizza>(plz::Margarita, plz::S));
    ASSERT_TRUE(kitchen.canAddPizzas(1));
    kitchen.addPizza(std::make_shared<plz::Pizza>(plz::Margarita, plz::S));
    ASSERT_FALSE(kitchen.canAddPizzas(1));
}

TEST(Kitchen, canAddPizzasFiveCooks) {
    plz::KitchenSettings settings;
    plz::Kitchen kitchen;

    settings.nbCooks = 5;
    kitchen.setSettings(settings);
    ASSERT_TRUE(kitchen.canAddPizzas(10));
    ASSERT_FALSE(kitchen.canAddPizzas(11));
}

TEST(Kitchen, canAddPizzasFiveCooks_pizzasAlreadyInQueue) {
    plz::KitchenSettings settings;
    plz::Kitchen kitchen;

    settings.nbCooks = 5;
    kitchen.setSettings(settings);
    for (size_t i = 0 ; i < 7 ; i++)
        kitchen.addPizza(std::make_shared<plz::Pizza>(plz::Margarita, plz::S));
    ASSERT_TRUE(kitchen.canAddPizzas(3));
    ASSERT_FALSE(kitchen.canAddPizzas(4));
}

TEST(Kitchen, inactiveTime) {
    plz::Kitchen kitchen;
    plz::KitchenSettings settings;
    std::chrono::milliseconds time(0);

    settings.inactiveTime = std::chrono::milliseconds(100);
    kitchen.setSettings(settings);
    auto t1 = std::chrono::steady_clock::now();
    kitchen.run();
    time = getElapsedTime(t1);
    ASSERT_TRUE((time >= settings.inactiveTime)
        && (time <= settings.inactiveTime + std::chrono::milliseconds(100)));
}

TEST(Kitchen, makeOnePizza_oneCook) {
    std::thread kitchenThread;
    std::chrono::milliseconds time(0);
    std::chrono::milliseconds expectedTime(0);
    std::unordered_map<std::string, size_t> stock;
    plz::KitchenSettings settings;
    size_t increment = 0;
    auto kitchen = std::make_shared<plz::Kitchen>();
    auto pizza = std::make_shared<plz::Pizza>(plz::Regina, plz::XXL);

    settings.cookingMultiplier = 1.0f;
    settings.nbCooks = 1;
    expectedTime = std::chrono::milliseconds(pizza->timeToBake
                                            + settings.inactiveTime);
    kitchen->setSettings(settings);
    kitchen->addPizza(pizza);
    auto t1 = std::chrono::steady_clock::now();
    kitchenThread = std::thread(&plz::Kitchen::run, kitchen);
    kitchenThread.join();
    time = getElapsedTime(t1);
    ASSERT_TRUE((time >= expectedTime)
        && (time <= expectedTime + std::chrono::milliseconds(500)));
    increment = time.count() / settings.restockTime.count();
    stock = kitchen->getStock();
    for (const auto & [ name, remaining ] : stock) {
        if (std::find(pizza->ingredients.begin(), pizza->ingredients.end(),
                        name) != pizza->ingredients.end()) {
            ASSERT_EQ(remaining, settings.startNbIngredients + increment - 1);
        } else {
            ASSERT_EQ(remaining, settings.startNbIngredients + increment);
        }
    }
}
