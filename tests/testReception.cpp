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

static void testPizzaQueue(plz::Reception::PizzaQueue_t &pizzaQueue,
                    size_t nbPizzas, plz::PizzaType type, plz::PizzaSize size) {
    std::shared_ptr<plz::Pizza> pizza = nullptr;

    for (size_t i = 0 ; i < nbPizzas ; i++) {
        pizza = pizzaQueue.front();
        ASSERT_EQ(pizza->type, type);
        ASSERT_EQ(pizza->size, size);
        pizzaQueue.pop();
    }
}

TEST(Reception, placeCorrectOrders) {
    plz::Reception reception;
    plz::Reception::PizzaQueue_t pizzaQueue;
    std::string orders = "regina XXL x2; fantasia M x3; margarita S x1";

    testing::internal::CaptureStdout();
    reception.placeOrders(orders);
    pizzaQueue = reception.getPizzaQueue();
    ASSERT_EQ(pizzaQueue.size(), 6);
    testPizzaQueue(pizzaQueue, 2, plz::PizzaType::Regina, plz::PizzaSize::XXL);
    testPizzaQueue(pizzaQueue, 3, plz::PizzaType::Fantasia, plz::PizzaSize::M);
    testPizzaQueue(pizzaQueue, 1, plz::PizzaType::Margarita, plz::PizzaSize::S);
}

TEST(Reception, placeCorrectOrder) {
    plz::Reception reception;
    plz::Reception::PizzaQueue_t pizzaQueue;
    std::string order = "Americana XL x3";

    testing::internal::CaptureStdout();
    ASSERT_TRUE(reception.placeOrder(order));
    pizzaQueue = reception.getPizzaQueue();
    ASSERT_EQ(pizzaQueue.size(), 3);
    testPizzaQueue(pizzaQueue, 3, plz::PizzaType::Americana, plz::PizzaSize::XL);
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

    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    ASSERT_FALSE(reception.placeOrder(order));
    stdoutOutput = testing::internal::GetCapturedStdout();
    stderrOutput = testing::internal::GetCapturedStderr();
    ASSERT_EQ(stdoutOutput, "Invalid order.\n");
    ASSERT_EQ(stderrOutput, expectedStderrOutput + "\n");
}
