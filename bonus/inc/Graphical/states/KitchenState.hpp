/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** KitchenState
*/

#ifndef KITCHENSTATE_HPP_
#define KITCHENSTATE_HPP_

#include "AState.hpp"
#include "Banner.hpp"
#include "Button.hpp"

namespace plz {
class KitchenState : public AState {
 public:
    explicit KitchenState(const struct StateData &stateData,
        const std::vector<pid_t> &pids);
    virtual ~KitchenState(void) {}

    void update(const millis_t dt) override;
    void render(void) override;

 private:
    void initAnnounceText(const sf::Vector2u &winSize);
    void initNextPizzaText(const sf::Vector2u &winSize,
        std::vector<std::string> &statuslines);
    void initCookBanners(const sf::Vector2u &winSize,
        std::vector<std::string> &statuslines);
    void initIngredientBanners(const sf::Vector2u &winSize,
        std::vector<std::string> &statuslines);
    void initBackButton(const sf::Vector2u &winSize);
    void initNextKitchenButton(const sf::Vector2u &winSize);

    void updateNextPizzaText(std::vector<std::string> &statuslines);
    void updateCookBanners(std::vector<std::string> &statuslines);
    void updateIngredientBanners(std::vector<std::string> &statuslines);
    void updateBackButton(void);
    void updateNextKitchenButton(void);

    pid_t _pid;
    bool _activated;
    std::vector<pid_t> _otherKitchensPids;
    millis_t _timePassed;
    sf::RectangleShape _background;
    sf::Text _announceText;
    sf::Text _nextPizzaText;
    std::map<int, std::unique_ptr<Banner>> _cookBanners;
    std::map<std::string, std::unique_ptr<Banner>> _ingredientBanners;
    std::unique_ptr<Button> _backButton;
    std::unique_ptr<Button> _nextKitchenButton;
};
}  // namespace plz

#endif /* !KITCHENSTATE_HPP_ */
