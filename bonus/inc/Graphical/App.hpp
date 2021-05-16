/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** App
*/

#ifndef INC_APP_HPP_
#define INC_APP_HPP_

#include <memory>
#include <stack>
#include "AppTypes.hpp"
#include "AState.hpp"
#include "Reception.hpp"

namespace plz {
//* Core app class
class App {
 public:
    //* Default constructor for the app
    App(void);

    //* Default destructor of the app
    virtual ~App(void) {}

    //* Launches the app !
    void run(void);

 private:
    //* Initializer function for the app window
    void initWindow(void);

    //* Initializer function for the states
    void initStates(void);

    //* Update the app
    void update(void);

    //* Render the app
    void render(void);

    //* Handles the event
    void handleEvents(void);

    std::shared_ptr<Reception> _reception;

    //* App window
    std::shared_ptr<Window_t> _win;

    //* App clock
    std::unique_ptr<Clock_t> _clock;

    //* States stack
    std::shared_ptr<std::stack<std::shared_ptr<AState>>> _states;
};
}  // namespace plz

#endif  // INC_APP_HPP_

