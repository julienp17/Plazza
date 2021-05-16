/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Pizza
*/

#include <iomanip>
#include <string.h>
#include <unordered_map>
#include "Pizza.hpp"
#include "Error.hpp"

/* ---------------------------- Static functions ---------------------------- */

static std::unordered_map<std::string, plz::PizzaType> getPizzaTypeMap(void) {
    std::unordered_map<std::string, plz::PizzaType> types;

    types["Regina"] = plz::PizzaType::Regina;
    types["Margarita"] = plz::PizzaType::Margarita;
    types["Americana"] = plz::PizzaType::Americana;
    types["Fantasia"] = plz::PizzaType::Fantasia;
    return types;
}

static std::unordered_map<std::string, plz::PizzaSize> getPizzaSizeMap(void) {
    std::unordered_map<std::string, plz::PizzaSize> sizes;

    sizes["S"] = plz::PizzaSize::S;
    sizes["M"] = plz::PizzaSize::M;
    sizes["L"] = plz::PizzaSize::L;
    sizes["XL"] = plz::PizzaSize::XL;
    sizes["XXL"] = plz::PizzaSize::XXL;
    return sizes;
}

static std::unordered_map<plz::PizzaType, std::chrono::milliseconds>
                                                getPizzaBakeTimeMap(void) {
    std::unordered_map<plz::PizzaType, std::chrono::milliseconds> bakingTimes;

    bakingTimes[plz::PizzaType::Regina] = std::chrono::milliseconds(2000);
    bakingTimes[plz::PizzaType::Margarita] = std::chrono::milliseconds(1000);
    bakingTimes[plz::PizzaType::Americana] = std::chrono::milliseconds(2000);
    bakingTimes[plz::PizzaType::Fantasia] = std::chrono::milliseconds(4000);
    return bakingTimes;
}

static std::unordered_map<plz::PizzaType, std::vector<std::string>>
                                                getPizzaIngredientsMap(void) {
    std::unordered_map<plz::PizzaType, std::vector<std::string>> ingredients;

    ingredients[plz::PizzaType::Regina] = std::vector<std::string>
        {"doe", "tomato", "gruyere", "ham", "mushrooms"};
    ingredients[plz::PizzaType::Margarita] = std::vector<std::string>
        {"doe", "tomato", "gruyere"};
    ingredients[plz::PizzaType::Americana] = std::vector<std::string>
        {"doe", "tomato", "gruyere", "steak"};
    ingredients[plz::PizzaType::Fantasia] = std::vector<std::string>
        {"doe", "tomato", "eggplant", "goat cheese", "chief love"};
    return ingredients;
}

namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */
Pizza::Pizza(const std::string &type, const std::string &size,
        const std::string &id) {
    this->type = getPizzaType(type);
    this->size = getPizzaSize(size);
    this->id = id;
    this->timeToBake = getPizzaBakeTime(this->type);
    this->ingredients = getPizzaIngredients(this->type);
}

Pizza::Pizza(const PizzaType type, const PizzaSize size,
        const std::string &id) {
    this->type = type;
    this->size = size;
    this->id = id;
    this->timeToBake = getPizzaBakeTime(type);
    this->ingredients = getPizzaIngredients(this->type);
}

/* ---------------------------- Getters / Setters --------------------------- */

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

std::chrono::milliseconds getPizzaBakeTime(const PizzaType type) {
    std::unordered_map<PizzaType, std::chrono::milliseconds> bakingTimes =
        getPizzaBakeTimeMap();

    if (bakingTimes.find(type) == bakingTimes.end())
        throw PizzaError(type + ": unknown pizza type.");
    return bakingTimes[type];
}

std::vector<std::string> getPizzaIngredients(const PizzaType type) {
    std::unordered_map<PizzaType, std::vector<std::string>> ingredients =
        getPizzaIngredientsMap();

    if (ingredients.find(type) == ingredients.end())
        throw PizzaError(type + ": unknown pizza type.");
    return ingredients[type];
}

}  // namespace plz

/* -------------------------- Operator overloading -------------------------- */

std::ostream &operator<<(std::ostream &out, const plz::PizzaType type) {
    std::string typeStr = plz::getPizzaType(type);

    return out << typeStr;
}

std::ostream &operator<<(std::ostream &out, const plz::PizzaSize size) {
    std::string sizeStr = plz::getPizzaSize(size);

    return out << sizeStr;
}

std::ostream &operator<<(std::ostream &out, const plz::Pizza &pizza) {
    out << pizza.type << " " << pizza.size;
    if (!pizza.id.empty()) {
        out << " " << pizza.id;
    }
    return out;
}
