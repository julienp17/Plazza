/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Pizza
*/

#include <string.h>
#include <unordered_map>
#include "Pizza.hpp"
#include "Error.hpp"

static std::unordered_map<std::string, plz::PizzaType> getPizzaTypeMap(void) {
    std::unordered_map<std::string, plz::PizzaType> types;

    types.emplace("Regina", plz::PizzaType::Regina);
    types.emplace("Margarita", plz::PizzaType::Margarita);
    types.emplace("Americana", plz::PizzaType::Americana);
    types.emplace("Fantasia", plz::PizzaType::Fantasia);
    return types;
}

static std::unordered_map<std::string, plz::PizzaSize> getPizzaSizeMap(void) {
    std::unordered_map<std::string, plz::PizzaSize> sizes;

    sizes.emplace("S", plz::PizzaSize::S);
    sizes.emplace("M", plz::PizzaSize::M);
    sizes.emplace("L", plz::PizzaSize::L);
    sizes.emplace("XL", plz::PizzaSize::XL);
    sizes.emplace("XXL", plz::PizzaSize::XXL);
    return sizes;
}

namespace plz {
Pizza::Pizza(const std::string &type, const std::string &size) {
    this->type = getPizzaType(type);
    this->size = getPizzaSize(size);
    this->timeToBake = getPizzaBakeTime(this->type);
}

Pizza::Pizza(const PizzaType type, const PizzaSize size) {
    this->type = type;
    this->size = size;
    this->timeToBake = getPizzaBakeTime(type);
}

PizzaType getPizzaType(const std::string &type) {
    std::unordered_map<std::string, PizzaType> types = getPizzaTypeMap();

    for (auto &it : types)
        if (strcasecmp(type.c_str(), it.first.c_str()) == 0)
            return it.second;
    throw PizzaError(type + ": unknown pizza type.");
}

std::string getPizzaType(const PizzaType type) noexcept {
    std::unordered_map<std::string, PizzaType> types = getPizzaTypeMap();

    for (auto &it : types)
        if (it.second == type)
            return it.first;
    return "Unknown pizza type.";
}

PizzaSize getPizzaSize(const std::string &size) {
    std::unordered_map<std::string, PizzaSize> sizes = getPizzaSizeMap();

    for (auto &it : sizes)
        if (strcasecmp(size.c_str(), it.first.c_str()) == 0)
            return it.second;
    throw PizzaError(size + ": unknown pizza size.");
}

std::string getPizzaSize(const PizzaSize size) noexcept {
    std::unordered_map<std::string, PizzaSize> sizes = getPizzaSizeMap();

    for (auto &it : sizes)
        if (it.second == size)
            return it.first;
    return "Unknown pizza size.";
}

millis_t getPizzaBakeTime(const PizzaType type) {
    std::unordered_map<PizzaType, millis_t> bakingTimes;

    bakingTimes.emplace(PizzaType::Regina, 2000);
    bakingTimes.emplace(PizzaType::Margarita, 1000);
    bakingTimes.emplace(PizzaType::Americana, 2000);
    bakingTimes.emplace(PizzaType::Fantasia, 4000);
    if (bakingTimes.find(type) == bakingTimes.end())
        throw PizzaError(type + ": unknown pizza type.");
    return bakingTimes[type];
}
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::PizzaType type) {
    std::string typeStr = plz::getPizzaType(type);

    return out << typeStr;
}

std::ostream &operator<<(std::ostream &out, const plz::PizzaSize size) {
    std::string sizeStr = plz::getPizzaSize(size);

    return out << sizeStr;
}

std::ostream &operator<<(std::ostream &out, const plz::Pizza &pizza) {
    return out << pizza.type << " " << pizza.size;
}
