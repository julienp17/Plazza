/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#ifndef INC_RECEPTION_HPP_
#define INC_RECEPTION_HPP_

#include <string>
#include <queue>
#include "plazza.hpp"
#include "Pizza.hpp"

namespace plz {
class Reception {
 public:
    Reception(void) {}
    virtual ~Reception(void) {}

    /**
     * @brief Add pizzas to the queue
     *
     * @param orders The string defining the orders, separated by the ';'
     * delimiter
     */
    void placeOrders(const std::string &orders);

    /**
     * @brief Adds a pizza to the queue
     *
     * @param order The string defining the order, following the "TYPE SIZE
     * NB_PIZZAS" format
     * @return true if the pizza was correctly added to the queue
     * @return false if the order was incorrect
     */
    bool placeOrder(const std::string &order);

    /**
     * @brief Get the pizza queue
     *
     * @return The pizza queue
     */
    inline const std::queue<Pizza> &getPizzaQueue(void) const {
       return _pizzas;
    }

 private:
    /**
     * @brief Checks if the order format is correct
     *
     * @param tokens The order tokens
     * @return true if the order is correct, false otherwise
     */
    bool orderIsCorrect(const VecStr_t &tokens);

    std::queue<Pizza> _pizzas;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
