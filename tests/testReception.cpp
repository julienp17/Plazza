/*
** EPITECH PROJECT, 2021
** nanoTekSpice
** File description:
** test4001Component
*/

#include <gtest/gtest.h>
#include <cstdio>
#include <vector>
#include "Reception.hpp"
#include "Error.hpp"

TEST(Reception, placeCorrectOrder) {
    plz::Reception reception;
    std::string order = "Regina S x3";
    std::queue<plz::Pizza> pizzaQueue;

    ASSERT_TRUE(reception.placeOrder(order));
    pizzaQueue = reception.getPizzaQueue();
    ASSERT_EQ(pizzaQueue.size(), 3);
    while (!pizzaQueue.empty()) {
        plz::Pizza pizza = pizzaQueue.front();
        ASSERT_EQ(pizza.type, plz::PizzaType::Regina);
        ASSERT_EQ(pizza.size, plz::PizzaSize::S);
        pizzaQueue.pop();
    }
}

TEST(Reception, placeCorrectOrder2) {
    plz::Reception reception;
    std::string order = "Fantasia XXL x1";
    std::queue<plz::Pizza> pizzaQueue;

    ASSERT_TRUE(reception.placeOrder(order));
    pizzaQueue = reception.getPizzaQueue();
    ASSERT_EQ(pizzaQueue.size(), 1);
    plz::Pizza pizza = pizzaQueue.front();
    ASSERT_EQ(pizza.type, plz::PizzaType::Fantasia);
    ASSERT_EQ(pizza.size, plz::PizzaSize::XXL);
}

class PlaceIncorrectOrders
    : public ::testing::TestWithParam<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_CASE_P(
    PlaceOrder,
    PlaceIncorrectOrders,
    ::testing::Values(
        std::make_tuple("Regina S",
                        "Pizza ordering must be: TYPE SIZE NUMBER."),
        std::make_tuple("Regina S x3 and a Coca Cola please",
                        "Pizza ordering must be: TYPE SIZE NUMBER."),
        std::make_tuple("Chevre-Miel XL x2",
                        "Chevre-Miel: unknown pizza type."),
        std::make_tuple("Margarita N x1",
                        "N: unknown pizza size."),
        std::make_tuple("Regina S x",
                        "The number of pizzas must be positive."),
        std::make_tuple("Regina S x0",
                        "The number of pizzas must be positive."),
        std::make_tuple("Regina S x-2",
                        "The number of pizzas must be positive.")
    )
);

TEST_P(PlaceIncorrectOrders, test) {
    plz::Reception reception;
    std::string order = std::get<0>(GetParam());
    std::string expectedStderrOutput = std::get<1>(GetParam());
    std::string stdoutOutput;
    std::string stderrOutput;

    testing::internal::CaptureStderr();
    testing::internal::CaptureStdout();
    ASSERT_FALSE(reception.placeOrder(order));
    stdoutOutput = testing::internal::GetCapturedStdout();
    stderrOutput = testing::internal::GetCapturedStderr();
    ASSERT_EQ(stdoutOutput, "Invalid order.\n");
    ASSERT_EQ(stderrOutput, expectedStderrOutput + "\n");
}
