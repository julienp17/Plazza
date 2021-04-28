/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** plazza
*/

#ifndef INC_PLAZZA_HPP_
#define INC_PLAZZA_HPP_

#include <vector>
#include <chrono>
#include "utils.hpp"

#define MY_EXIT_FAILURE 84
#define USAGE \
"Usage: plazza COOKING_TIME NB_COOKS STOCK_TIME\n" \
"    COOKING_TIME: Positive number representing the multiplier for " \
"the cooking time of the pizzas.\n" \
"        NB_COOKS: Positive integer representing the number of cooks " \
"per kitchen.\n" \
"      STOCK_TIME: Positive integer representing the time in milliseconds " \
"used by the kitchen stock to replace ingredients."
#define exitUsage(stream, exitCode) \
    do { stream << USAGE << std::endl; return exitCode; } while (0)

namespace plz {
typedef std::chrono::milliseconds milliseconds_t;
}

#endif  // INC_PLAZZA_HPP_
