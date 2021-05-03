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
     * Does nothing
     */
    virtual ~Kitchen(void) {}

    /**
     * @brief Main loop of the kitchen
     */
    void loop(void);

    /**
     * @brief Restock the kitchen
     *
     * Regenerates N unit of each ingredients, with N being the number specified
     * by settings.restockNb
     */
    void restock(void);

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
     * @brief Initialize the kitchen, called by the constructors
     *
     */
    void init(void);

    /**
     * @brief Initialize the kitchen stock
     */
    void initStock(void);

    //* The settings of the kitchen
    KitchenSettings _settings;

    //* Mapping of ingredients to their remaining stock
    std::unordered_map<std::string, size_t> _stock;

    //* Restocking timepoint
    std::chrono::time_point<std::chrono::steady_clock> _restockTimepoint;

    //* Active timepoint
    std::chrono::time_point<std::chrono::steady_clock> _activeTimepoint;
};
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen);

#endif  // INC_KITCHEN_HPP_
