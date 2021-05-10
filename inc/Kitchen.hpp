/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#ifndef INC_KITCHEN_HPP_
#define INC_KITCHEN_HPP_

#include <string>
#include <memory>
#include <chrono>

#include <unordered_map>
#include <queue>
#include <vector>
#include <utility>

#include <atomic>
#include <thread>
#include <mutex>

#include "utils.hpp"
#include "MessageQueue.hpp"
#include "KitchenSettings.hpp"
#include "Pizza.hpp"
#include "Cook.hpp"

namespace plz {
/**
 * @brief Produces pizzas for the reception made by concurrent cooks
 */
class Kitchen {
 public:
    /**
     * @brief Default constructor for a kitchen
     *
     * Creates a kitchen with default settings
     */
    Kitchen(void);

    /**
     * @brief Construct a new Kitchen object with specified settings
     *
     * @param settings The settings of the kitchen to create
     */
    Kitchen(const KitchenSettings &settings, std::shared_ptr<MessageQueue>);

    /**
     * @brief Destroy the Kitchen object
     *
     * Waits for every cook to finish their shift
     */
    virtual ~Kitchen(void);

    /**
     * @brief Main loop of the kitchen
     */
    void run(void);

    /**
     * @brief Adds a pizza to the queue
     *
     * @param pizza The pizza to be made
     */
    void addPizza(std::shared_ptr<Pizza> pizza);

    /**
     * @brief Restock the kitchen
     *
     * Regenerates N unit of each ingredients, with N being the number specified
     * by settings.restockNb
     */
    void restock(void);

    /**
     * @brief Checks whether the kitchen can add a pizza to its queue.
     *
     * The kitchen cannot accept more than 2 * N pizza, with N being the number
     * of cooks.
     *
     * @param nbPizzas The requested number of pizzas to add
     * @return true if the kitchen can accept the pizzas
     * @return false otherwise
     */
    bool canAddPizzas(const size_t nbPizzas);

    /**
     * @brief Checks whether the kitchen can make the passed pizza
     *
     * The kitchen can only make the pizza if it has enough ingredients.
     *
     * @param pizza The pizza to make
     * @return true if the kitchen has enough ingredients
     * @return false otherwise
     */
    bool canMakePizza(const std::shared_ptr<Pizza> pizza);

    /**
     * @brief Get the cooks of the kitchen
     * 
     * @return A vector of shared pointers to the cooks
     */
    std::vector<std::shared_ptr<Cook>> getCooks(void) const;

    /**
     * @brief Get the kitchen's settings
     *
     * @return The settings of the kitchen
     */
    const KitchenSettings &getSettings(void) const {
        return _settings;
    }

    /**
     * @brief Sets new settings for the kitchen
     *
     * @param settings The new settings for the kitchen
     */
    void setSettings(const KitchenSettings &settings);

    /**
     * @brief Get the mapping of ingredients to their remaining stock
     *
     * @return A map of ingredients name as std::string, to their stock as size_t
     */
    inline const std::unordered_map<std::string, size_t> &getStock(void) const {
        return _stock;
    }

 private:
    /**
     * @brief Initialize the kitchen
     *
     * This function is called by the constructors.
     * This function calls the other initialization functions.
     */
    void init(void);

    /**
     * @brief Initialize the kitchen's stock
     *
     * Puts N number of each ingredient in the stock, with N being the number
     * specified in settings.startNbIngredients
     */
    void initStock(void);

    /**
     * @brief Initiliaze the kitchen's cooks
     *
     * Creates N number of cooks with an increasing ID and random name, with N
     * being the number specified in settings.nbCooks
     */
    void initCooks(void);

    /**
     * @brief Checks if the kitchen is active.
     *
     * A kitchen is considered active if it got an order or a cook is making
     * a pizza.
     *
     * @return true if the kitchen is active
     * @return false otherwise
     */
    bool isActive(void) const;

    /**
     * @brief Checks if the kitchen should restock
     *
     * @return true if its time to restock, false otherwise
     */
    bool shouldRestock(void) const;

    /**
     * @brief Checks if the kitchen should close
     *
     * @return true if the kitchen has been inactive for enough time,
     * false otherwise
     */
    bool shouldClose(void) const;

    /**
     * @brief Gets the number of available cooks
     */
    size_t getNbAvailableCooks(void) const;

    /**
     * @brief Consumes one piece of each ingredients from the stock
     *
     * @param ingredients The name of each ingredient to consume
     */
    void useIngredients(const std::vector<std::string> &ingredients);

    /**
     * @brief Checks if a command was received from the message queue, and
     * calls the corresponding action
     */
    void handleReceived(void);

    /**
     * @brief Checks if the received order is possible to make, and send a
     * response to the reception
     */
    void respondOrder(const std::string &message);

    /**
     * @brief Sends the status of the kitchen to the reception
     */
    void respondStatus(const std::string &message);

    /**
     * @brief Starts every cook's thread with the cookWorker member function
     */
    void putCooksToWork(void);

    /**
     * @brief Thread function for each cook
     *
     * Checks if a pizza is ready to be made from the queue.
     * If it is, and there is enough ingredients, sleep for the amount of time
     * the pizza needs to be baked.
     * Else, yield.
     *
     * @param settings The working cook for the thread
     */
    void cookWorker(std::shared_ptr<Cook> cook);

    /**
     * @brief Checks if the kitchen is currently active
     *
     * A kitchen closes when it is inactive for N amount of time, with N being
     * the number specified in the kitchen's settings. Inactive means that there
     * are no pizza orders received or no cooks working in that amount of time.
     */
    std::atomic<bool> _isOpen;

    //* The cooks of the kitchen, represented by threads
    std::vector<std::pair<std::shared_ptr<Cook>, std::thread>> _cooks;

    //* The queue of pizzas to be made
    std::queue<std::shared_ptr<Pizza>> _pizzaQueue;

    //* Mapping of ingredients to their remaining stock
    std::unordered_map<std::string, size_t> _stock;

    //* The mutex used for accessing the queue
    std::mutex _queueMutex;

    //* The mutex used for consumming stocks
    std::mutex _stockMutex;

    //* The mutex used for accessing the messageQueue
    std::mutex _msgQueueMutex;

    //* Timepoint to the last time the kitchen restocked
    std::chrono::time_point<std::chrono::steady_clock> _lastRestock;

    //* The timepoint at where the kitchen was last active
    std::chrono::time_point<std::chrono::steady_clock> _lastActive;

    //* The message queue used to communicate with the Reception
    std::shared_ptr<MessageQueue> _msgQueue;

    //* The settings of the kitchen
    KitchenSettings _settings;
};
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen);

#endif  // INC_KITCHEN_HPP_
