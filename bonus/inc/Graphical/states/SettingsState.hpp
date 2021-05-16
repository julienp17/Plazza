/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** SettingsState
*/

#ifndef INC_SETTINGSSTATE_HPP_
#define INC_SETTINGSSTATE_HPP_

#include <unordered_map>
#include "AState.hpp"
#include "Counter.hpp"
#include "KitchenSettings.hpp"

namespace plz {
class SettingsState : public AState {
 public:
    explicit SettingsState(const struct StateData &stateData);
    virtual ~SettingsState(void) {}

    void update(const millis_t dt) override;
    void render(void) override;

 private:
    void initCookingMultiplier(const sf::Vector2u &winSize);
    void initNbCooks(const sf::Vector2u &winSize);
    void initRestockTime(const sf::Vector2u &winSize);
    void initBackButton(const sf::Vector2u &winSize);

    void updateCookingMultiplier(void);
    void updateNbCooks(void);
    void updateRestockTime(void);
    void updateBackButton(void);

    KitchenSettings _settings;
    std::unique_ptr<Button> _backButton;
    std::unique_ptr<Counter<float>> _cookingMultiplier;
    std::unique_ptr<Counter<int>> _nbCooks;
    std::unique_ptr<Counter<int>> _restockTime;
};
}  // namespace plz


#endif  // INC_SETTINGSSTATE_HPP_
