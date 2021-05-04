/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#ifndef INC_KITCHEN_HPP_
#define INC_KITCHEN_HPP_

#include <unordered_map>
#include <string>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "utils.hpp"

namespace plz {
/**
 * @brief Settings for a kitchen
 */
struct KitchenSettings {
    /**
     * @brief Construct a new Kitchen Settings object with default values
     */
    KitchenSettings(void);

    /**
     * @brief Multiplier for the cooking time of the pizzas
     */
    float cookingMultiplier;

    /**
     * @brief Number of cooks in the kitchen
     */
    size_t nbCooks;

    /**
     * @brief The starting stock of each ingredient
     */
    size_t startNbIngredients;

    /**
     * @brief Time for a kitchen stock to replace ingredients
     */
    std::chrono::milliseconds restockTime;

    /**
     * @brief Maximum time for a kitchen to be inactive before it closes
     */
    std::chrono::milliseconds inactiveTime;

    /**
     * @brief The number to restock for each ingredient
     */
    size_t restockNb;
};

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
     * @brief Restock the kitchen
     *
     * Regenerates N unit of each ingredients, with N being the number specified
     * by settings.restockNb
     */
    void restock(void);

    /**
     * @brief Consume an ingredient from the stock
     *
     * @param name The ingredient's name
     */
    void useIngredient(const std::string &name);

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
    void setSettings(const KitchenSettings &settings) {
        _settings = settings;
    }

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
     * This function is called by the constructors
     */
    void init(void);

    /**
     * @brief Initialize the kitchen stock
     *
     * Puts N number of each ingredient in the stock, with N being the number
     * specified in settings.startNbIngredients
     */
    void initStock(void);

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
     * @brief Thread function for each cook
     *
     * Checks if a pizza is ready to be made from the queue. If it is, sleep
     * for the amount of time the pizza needs to be baked, else yield.
     *
     */
    void cookWorker(void);

    /**
     * @brief Starts every cook's thread with the cookWorker member function
     */
    void putCooksToWork(void);

    /**
     * @brief Checks if the kitchen is currently active
     *
     * A kitchen closes when it is inactive for N amount of time, with N being
     * the number specified in the kitchen's settings. Inactive means that there
     * are no pizza orders received or no cooks working in that amount of time.
     */
    std::atomic<bool> _isOpen;

    //* The cooks of the kitchen, represented by threads
    std::vector<std::thread> _cooks;

    //* Mapping of ingredients to their remaining stock
    std::unordered_map<std::string, size_t> _stock;

    //* The mutex used for consumming stocks
    std::mutex _stockMutex;

    //* Timepoint to the last time the kitchen restocked
    std::chrono::time_point<std::chrono::steady_clock> _restockTimepoint;

    //* The timepoint at where the kitchen was last active
    std::chrono::time_point<std::chrono::steady_clock> _activeTimepoint;

    //* The settings of the kitchen
    KitchenSettings _settings;
};
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen);

#endif  // INC_KITCHEN_HPP_
