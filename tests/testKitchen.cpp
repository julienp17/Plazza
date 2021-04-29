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
    plz::Kitchen kitchen;
    std::unordered_map<std::string, size_t> stock;

    stock = kitchen.getStock();
    for (const auto & [ name, remaining ] : stock)
        ASSERT_EQ(remaining, 5);
}
