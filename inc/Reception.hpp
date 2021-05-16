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
#include <map>
#include "plazza.hpp"
#include "Pizza.hpp"
#include "Kitchen.hpp"
#include "MessageQueue.hpp"

namespace plz {
class Reception {
 public:
    //* Construct a new Reception object
    Reception(void);

    //* Destroy the Reception object
    virtual ~Reception(void);

    //* Main loop of the reception
    void run(void);

    //* Wait for every kitchen process to finish
    void close(void);

    //* Print the status of each kitchen
    void printStatus(void);

    //* Print the list of all commands
    void printHelp(void);

    /**
     * @brief Adds a pizza to the queue
     *
     * @param order The string defining the order, following the "TYPE SIZE
     * NB_PIZZAS" format
     * @return true if the pizza was correctly added to the queue
     * @return false if the order was incorrect
     */
    bool placeOrder(const std::string &order);

    //* Get the settings of the kitchens
    inline KitchenSettings getKitchenSettings(void) const {
        return _kitchenSettings;
    }

    //* Set new settings for kitchens
    inline void setKitchenSettings(const KitchenSettings &kitchenSettings) {
        _kitchenSettings = kitchenSettings;
    }

 private:
    /**
     * @brief Do commands related to the reception
     *
     * Commands may be either pizza orders, status about the restaurant, or exit
     * 
     * @param commands The string defining the commands, separated by the ';'
     * delimiter
     */
    void doCommands(const std::string &commands);

    /**
     * @brief Forks a new process holding a kitchen
     *
     * The reception and the newly created kitchen will then communicate
     * using the message queue IPC.
     *
     * @return The pid of the new kitchen process
     */
    pid_t createKitchen(void);

    /**
     * @brief Checks if the order format is correct
     *
     * @param tokens The order tokens
     * @return true if the order is correct, false otherwise
     */
    bool orderIsCorrect(const VecStr_t &tokens);

    //* Delegate an order to a kitchen, creating one if none is available
    void delegateOrder(const std::string &order);

    /**
     * @brief Sends an order to a kitchen
     *
     * @param msgQueue The message queue of the kitchen
     * @param order The order to send
     * @return true if the order was accepted, false otherwise
     */
    bool sendOrder(std::shared_ptr<MessageQueue> msgQueue,
                    const std::string &order);

    //* Checks the messages received by kitchens, and act accordingly
    void handleReceived(void);

    //* Checks if a kitchen closed, and act accordingly
    void handleDisconnections(void);

    //* Settings for the kitchens of the reception
    KitchenSettings _kitchenSettings;

    //* Mapping of kitchen PIDs to their message queue
    std::map<pid_t, std::shared_ptr<MessageQueue>> _msgQueues;

    // Current ID of the order
    size_t _orderID;
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
