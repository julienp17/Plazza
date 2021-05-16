/*
** EPITECH PROJECT, 2021
** HorrorRPG [WSL: Ubuntu-20.04]
** File description:
** ReceptionState
*/

#ifndef INC_STATES_RECEPTIONSTATE_HPP_
#define INC_STATES_RECEPTIONSTATE_HPP_

#include <memory>
#include "AState.hpp"
#include "Button.hpp"
#include "Counter.hpp"

namespace plz {
class ReceptionState : public AState {
 public:
    explicit ReceptionState(const struct StateData &stateData);
    virtual ~ReceptionState(void) {}

    void update(const millis_t dt) override;
    void render(void) override;

 private:
    void initAnnounceText(const sf::Vector2u &winSize);
    void initPizzaButtons(const sf::Vector2u &winSize);
    void initSizeButtons(const sf::Vector2u &winSize);
    void initCounter(const sf::Vector2u &winSize);
    void initSendButton(const sf::Vector2u &winSize);
    void initBackButton(const sf::Vector2u &winSize);
    void initKitchenButton(const sf::Vector2u &winSize);

    void updateAnnouncement(void);
    void updateSelectedPizza(void);
    void updateSelectedSize(void);
    void updateCounter(void);
    void updateSendButton(void);
    void updateBackButton(void);
    void updateKitchenButton(void);

    sf::RectangleShape _background;
    sf::Text _announceText;
    std::shared_ptr<Button> _selectedPizza;
    std::unordered_map<std::string, std::shared_ptr<Button>> _pizzaButtons;
    std::shared_ptr<Button> _selectedSize;
    std::unordered_map<std::string, std::shared_ptr<Button>> _sizeButtons;
    std::unique_ptr<Counter<int>> _counter;
    std::unique_ptr<Button> _sendButton;
    std::unique_ptr<Button> _backButton;
    std::unique_ptr<Button> _kitchenButton;
};
}  // namespace plz

#endif  // INC_STATES_RECEPTIONSTATE_HPP_

