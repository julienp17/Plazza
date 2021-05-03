/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#ifndef INC_RECEPTION_HPP_
#define INC_RECEPTION_HPP_

#include <string>
#include <memory>
#include <queue>
#include "plazza.hpp"
#include "Pizza.hpp"
#include "Kitchen.hpp"

namespace plz {
class Reception {
 public:
    Reception(void) {}
    virtual ~Reception(void) {}

    using PizzaQueue_t = std::queue<std::shared_ptr<Pizza>>;

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
     * @brief Set the Kitchen Settings object
     * 
     * @param settings The new kitchen settings
     */
    void setKitchenSettings(const KitchenSettings &kitchenSettings) {
       _kitchenSettings = kitchenSettings;
    }

    /**
     * @brief Get the pizza queue
     *
     * @return The pizza queue
     */
    inline const PizzaQueue_t &getPizzaQueue(void) const {
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

    /**
     * @brief Settings for the kitchens of the reception
     */
    KitchenSettings _kitchenSettings;

    /**
     * @brief The queue of pizzas to be made
     */
    PizzaQueue_t _pizzas;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
