/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Cook
*/

#ifndef INC_COOK_HPP_
#define INC_COOK_HPP_

#include <thread>
#include <queue>
#include <memory>
#include <string>
#include "Pizza.hpp"

namespace plz {
class Cook {
 public:
    /**
     * @brief Creates a new cook
     *
     * @param id The id of the cook
     * @param name (optionnal) The name of the cook
     */
    explicit Cook(const size_t id, const std::string &name = "Cook")
        : _id(id), _name(name), _pizza(nullptr) {}

    /**
     * @brief Destroy the Cook object
     */
    virtual ~Cook(void) {}

    /**
     * @brief Get the id of the cook
     *
     * @return size_t The id of the cook
     */
    inline size_t getID(void) const {
        return _id;
    }

    /**
     * @brief Get the name of the cook
     *
     * @return const std::string& The name of the cook
     */
    inline const std::string &getName(void) const {
        return _name;
    }

    /**
     * @brief Checks whether the cook is currently working on a pizza.
     *
     * @return true if the cook is working on a pizza, false otherwise
     */
    inline bool isWorking(void) const {
        return _pizza == nullptr;
    }

    /**
     * @brief Sets the new pizza the cook should be working on
     *
     * @param pizza The new pizza to be worked on
     */
    void setPizza(std::shared_ptr<Pizza> pizza) {
        _pizza = pizza;
    }

    /**
     * @brief Make a new pizza
     *
     * @param multiplier The time multiplier for the baking time of the Pizzas
     */
    void makePizza(const float multiplier) {
        std::this_thread::sleep_for(_pizza->timeToBake * multiplier);
        _pizza = nullptr;
    }

 private:
    //* The id of the cook
    size_t _id;

    //* The name of the cook
    std::string _name;

    //* The current pizza that the cook is working on
    std::shared_ptr<Pizza> _pizza;
};
}  // namespace plz


#endif  // INC_COOK_HPP_
