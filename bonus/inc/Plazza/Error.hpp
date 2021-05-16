/*
** EPITECH PROJECT, 2021
** Arcade [WSL: Ubuntu-20.04]
** File description:
** ARCError
*/

#ifndef INC_ERROR_HPP_
#define INC_ERROR_HPP_

#include <exception>
#include <string>
#include <string.h>

namespace plz {
/** @class Error
 * @brief Standard arcade error
 */
class Error : public std::exception {
 public:
    explicit Error(std::string const &message)
        : _message(std::string("plazza: ") + message) {}

    inline virtual const char* what() const noexcept {
        return _message.c_str();
    }

 private:
    std::string _message;
};

/** @class ReceptionError
 * @brief Errors related to the reception
 */
class ReceptionError : public Error {
 public:
    ReceptionError(void)
        : Error(std::string("reception: ") + strerror(errno)) {}
    explicit ReceptionError(std::string const &message)
        : Error(std::string("reception: ") + message) {}
};

/** @class KitchenError
 * @brief Errors related to kitchens
 */
class KitchenError : public Error {
 public:
    explicit KitchenError(std::string const &message)
        : Error(std::string("kitchen: ") + message) {}
};

/** @class PizzaError
 * @brief Errors related to pizzas
 */
class PizzaError : public Error {
 public:
    explicit PizzaError(std::string const &message)
        : Error(std::string("pizza: ") + message) {}
};
}  // namespace plz

#endif  // INC_ERROR_HPP_
