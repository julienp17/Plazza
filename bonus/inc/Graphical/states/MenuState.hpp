/*
** EPITECH PROJECT, 2021
** HorrorRPG [WSL: Ubuntu-20.04]
** File description:
** MenuState
*/

#ifndef INC_STATES_MENUSTATE_HPP_
#define INC_STATES_MENUSTATE_HPP_

#include <memory>
#include <unordered_map>
#include "AState.hpp"
#include "Button.hpp"

namespace plz {
class MenuState : public AState {
 public:
    explicit MenuState(const struct StateData &stateData);
    virtual ~MenuState(void) {}

    void update(const millis_t dt) override;
    void render(void) override;

 private:
    std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;
};
}  // namespace plz

#endif  // INC_STATES_MENUSTATE_HPP_

