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

#define START_NB_INGREDIENTS        5

namespace plz {
/**
 * @brief Produces pizzas for the reception made by concurrent cooks
 */
class Kitchen {
 public:
    Kitchen(void);
    virtual ~Kitchen(void) {}

    /**
     * @brief Main loop of the kitchen
     */
    void loop(void);

    /**
     * @brief Restock the kitchen, by regenerating 1 unit of each ingredients
     */
    void restock(void);

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
     * @brief Restock the ingredients upon kitchen creation
     *
     * @param startNb The starting number of each ingredients
     */
    void initStock(const size_t startNb);

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
