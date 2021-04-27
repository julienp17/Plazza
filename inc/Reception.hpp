/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#ifndef INC_RECEPTION_HPP_
#define INC_RECEPTION_HPP_

#include <string>
#include "plazza.hpp"

namespace plz {
class Reception {
 public:
    Reception(void) {}
    virtual ~Reception(void) {}

    void placeOrders(void);
    void placeOrder(const std::string &order);
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
