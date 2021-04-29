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

namespace plz {
class Reception {
 public:
    Reception(void);
    Reception(const float cookingMultiplier, const size_t nbCooks,
            const millis_t stockTime);
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
     * @brief Get the pizza queue
     *
     * @return The pizza queue
     */
    inline const PizzaQueue_t &getPizzaQueue(void) const {
       return _pizzas;
    }

    /**
     * @brief Set the cooking multipilier
     *
     * @param cookingMultiplier The new cooking multiplier
     */
    inline void setCookingMultipilier(const float cookingMultiplier) {
       _cookingMultiplier = cookingMultiplier;
    }

    /**
     * @brief Set the number of cooks per kitchen
     *
     * @param nbCooks The new number of cooks per kitchen
     */
    inline void setNbCooks(const size_t nbCooks) {
       _nbCooks = nbCooks;
    }

    /**
     * @brief Set the time needed for a kitchen to recharge their stock
     * 
     * @param stockTime The new stocking time
     */
    inline void setStockTime(const millis_t stockTime) {
       _stockTime = stockTime;
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
     * @brief Multiplier for the cooking time of the pizzas
     */
    float _cookingMultiplier;

    /**
     * @brief Number of cooks per kitchen
     */
    size_t _nbCooks;

    /**
     * @brief Time for a kitchen stock to replace ingredients
     */
    millis_t _stockTime;

    /**
     * @brief The queue of pizzas to be made
     */
    PizzaQueue_t _pizzas;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
