/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** AState
*/

#ifndef INC_ASTATE_HPP_
#define INC_ASTATE_HPP_

#include <vector>
#include <memory>
#include <stack>
#include <unordered_map>
#include "AppTypes.hpp"
#include "Reception.hpp"

namespace plz {
class AState;
struct StateData {
    std::shared_ptr<Window_t> window;
    std::shared_ptr<Font_t> font;
    std::shared_ptr<Reception> reception;
    std::shared_ptr<std::stack<std::shared_ptr<AState>>> states;
};
class AState {
 public:
    AState(void):
        _win(nullptr), _font(nullptr), _states(nullptr), _reception(nullptr) {}
    virtual ~AState(void) {}

    virtual void update(const millis_t dt) {(void) dt;}
    virtual void render(void) {}

 protected:
    explicit AState(const struct StateData &stateData) {
        _win = stateData.window;
        _font = stateData.font;
        _reception = stateData.reception;
        _states = stateData.states;
        _stateData = stateData;
    }

    struct StateData _stateData;
    std::shared_ptr<Window_t> _win;
    std::shared_ptr<Font_t> _font;
    std::shared_ptr<std::stack<std::shared_ptr<AState>>> _states;
    std::shared_ptr<Reception> _reception;
    std::unordered_map<std::string, Texture_t> _textures;
};
}  // namespace plz

#endif  // INC_ASTATE_HPP_
