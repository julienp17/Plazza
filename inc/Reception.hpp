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
#include "MessageQueue.hpp"

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
     * @brief Forks a new process holding a kitchen
     *
     * The reception and the newly created kitchen will then communicate
     * using the message queue IPC.
     */
    void createKitchen(void);

    /**
     * @brief Get the pizza queue
     *
     * @return std::queue<std::shared_ptr<Pizza>> The pizza queue
     */
    inline const std::queue<std::shared_ptr<Pizza>> &getPizzaQueue(void) const {
        return _pizzas;
    }

    /**
     * @brief Get the Kitchen Settings object
     *
     * @return KitchenSettings The settings of the created kitchens
     */
    inline KitchenSettings getKitchenSettings(void) const {
        return _kitchenSettings;
    }

    /**
     * @brief Set the Kitchen Settings object
     * 
     * @param settings The new kitchen settings
     */
    inline void setKitchenSettings(const KitchenSettings &kitchenSettings) {
        _kitchenSettings = kitchenSettings;
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

    //* Mapping of kitchen PIDs to their message queue
    std::unordered_map<pid_t, MessageQueue> _msgQueues;

    /**
     * @brief The queue of pizzas to be made
     */
    std::queue<std::shared_ptr<Pizza>> _pizzas;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
