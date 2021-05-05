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
#include "KitchenSettings.hpp"
#include "Pizza.hpp"
#include "Cook.hpp"

std::ostream &operator<<(std::ostream &out, const plz::Cook &cook);

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
    explicit Kitchen(const KitchenSettings &settings);

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
    void addPizza(std::shared_ptr<Pizza> pizza) {
        _pizzaQueue.push(pizza);
    }

    /**
     * @brief Restock the kitchen
     *
     * Regenerates N unit of each ingredients, with N being the number specified
     * by settings.restockNb
     */
    void restock(void);

    /**
     * @brief Get the cooks of the kitchen
     * 
     * @return A vector of shared pointers to the cooks
     */
    std::vector<std::shared_ptr<Cook>> getCooks(void) const {
        std::vector<std::shared_ptr<Cook>> cooks;

        for (auto &pair : _cooks)
            cooks.push_back(pair.first);
        return cooks;
    }

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

    /**
     * @brief Overloading the stream operator for printing a kitchen
     * 
     * @param out The stream to output to
     * @param kitchen The kitchen to print
     * @return std::ostream& The modified stream
     */
    // friend std::ostream &operator<<(std::ostream &out,
    //                                 const plz::Kitchen &kitchen);

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
     * @brief Checks whether the kitchen can add a pizza to its queue.
     *
     * The kitchen cannot accept more than 2 * N pizza, with N being the number
     * of cooks.
     *
     * @return true if the kitchen has enough space
     * @return false otherwise
     */
    // bool canAddPizza(void) const;

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
     * @brief Consumes one piece of each ingredients from the stock
     *
     * @param ingredients The name of each ingredient to consume
     */
    void useIngredients(const std::vector<std::string> &ingredients);

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
     * @brief Returns the cook specified by id
     *
     * @param id The id of the cook
     * @return std::shared_ptr<Cook> A shared pointer to the cook
     */
    std::shared_ptr<Cook> getCook(const size_t id);

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

    //* The mutex used for printing to stdout
    std::mutex _stdoutMutex;

    //* Timepoint to the last time the kitchen restocked
    std::chrono::time_point<std::chrono::steady_clock> _lastRestock;

    //* The timepoint at where the kitchen was last active
    std::chrono::time_point<std::chrono::steady_clock> _lastActive;

    //* The settings of the kitchen
    KitchenSettings _settings;
};
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen);

#endif  // INC_KITCHEN_HPP_
