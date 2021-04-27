/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Pizza
*/

#include <unordered_map>
#include "Pizza.hpp"
#include "Error.hpp"

namespace plz {
Pizza::Pizza(const std::string &pType, const std::string &size) {
    this->type = getPizzaType(pType);
    this->size = getPizzaSize(size);
    this->timeToBake = getPizzaBakeTime(this->type);
}

PizzaType getPizzaType(const std::string &type) {
    std::unordered_map<std::string, PizzaType> types;

    types.emplace("Regina", PizzaType::Regina);
    types.emplace("Margarita", PizzaType::Margarita);
    types.emplace("Americana", PizzaType::Americana);
    types.emplace("Fantasia", PizzaType::Fantasia);
    if (types.find(type) == types.end())
        throw PizzaError(type + ": unknown pizza type.");
    return types[type];
}

PizzaSize getPizzaSize(const std::string &size) {
    std::unordered_map<std::string, PizzaSize> sizes;

    sizes.emplace("S", PizzaSize::S);
    sizes.emplace("M", PizzaSize::M);
    sizes.emplace("L", PizzaSize::L);
    sizes.emplace("XL", PizzaSize::XL);
    sizes.emplace("XXL", PizzaSize::XXL);
    if (sizes.find(size) == sizes.end())
        throw PizzaError(size + ": unknown pizza size.");
    return sizes[size];
}

milliseconds_t getPizzaBakeTime(const PizzaType type) {
    std::unordered_map<PizzaType, milliseconds_t> bakingTimes;

    bakingTimes.emplace(PizzaType::Regina, 2000);
    bakingTimes.emplace(PizzaType::Margarita, 1000);
    bakingTimes.emplace(PizzaType::Americana, 2000);
    bakingTimes.emplace(PizzaType::Fantasia, 4000);
    if (bakingTimes.find(type) == bakingTimes.end())
        throw PizzaError(type + ": unknown pizza type.");
    return bakingTimes[type];
}
}  // namespace plz
