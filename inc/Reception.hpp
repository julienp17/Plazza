/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#ifndef INC_RECEPTION_HPP_
#define INC_RECEPTION_HPP_

#include <string>
#include <vector>
#include "plazza.hpp"
#include "Pizza.hpp"

namespace plz {
class Reception {
 public:
    Reception(void) {}
    virtual ~Reception(void) {}

    /**
     * @brief Run the pizzeria
     */
    void run(void);

    /**
     * @brief Get a line from standard input and add pizzas to the queue
     */
    void placeOrders(void);

    /**
     * @brief Add a pizza to the queue
     *
     * @param order The string defining the order
     */
    void placeOrder(const std::string &order);

 private:
    /**
     * @brief Checks if the order format is correct
     *
     * @param tokens The order tokens
     * @return true if the order is correct, false otherwise
     */
    bool orderIsCorrect(const VecStr_t &tokens);

    std::vector<Pizza> _pizzas;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
