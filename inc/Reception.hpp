/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#ifndef INC_RECEPTION_HPP_
#define INC_RECEPTION_HPP_

#include "plazza.hpp"

namespace plz {
class Reception {
 public:
    Reception(void) {}
    virtual ~Reception(void) {}

    void placeOrder(void);
};
}  // namespace plz

#endif  // INC_RECEPTION_HPP_
