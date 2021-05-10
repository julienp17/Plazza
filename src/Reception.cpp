/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "Reception.hpp"
#include "Error.hpp"
#include "utils.hpp"

volatile sig_atomic_t isOpen = true;

void onInterrupt(int signum) {
    (void)signum;
    isOpen = false;
}

static void setSigIntHandler(void) {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = onInterrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */

Reception::~Reception(void) {
    for (auto &[pid, _] : this->_msgQueues)
        waitpid(pid, NULL, 0);
}

/* ---------------------------- Member functions ---------------------------- */

void Reception::run(void) {
    std::string commands;

    setSigIntHandler();
    std::cout << "Welcome to Plazza! The Pizzeria for everyone!" << std::endl;
    while (isOpen) {
        std::cout << "Place your order here: ";
        getline(std::cin, commands);
        this->doCommands(commands);
    }
    std::cout << "Closing the restaurant..." << std::endl;
}

void Reception::doCommands(const std::string &commands_str) {
    VecStr_t commands;

    commands = split(commands_str, ';');
    for (const std::string &order : commands) {
        this->handleReceived();
        if (strcasecmp(order.c_str(), "status") == 0)
            this->printStatus();
        else if (strcasecmp(order.c_str(), "exit") == 0)
            isOpen = false;
        else
            this->placeOrder(order);
    }
}

void Reception::printStatus(void) {
    for (auto &[pid, msgQueue] : this->_msgQueues) {
        msgQueue->send(ASK_STATUS, "Status");
        std::cout << msgQueue->recv(STATUS) << std::endl;
    }
    if (this->_msgQueues.empty())
        std::cout << "No kitchen running at the moment." << std::endl;
}

bool Reception::placeOrder(const std::string &order) {
    std::shared_ptr<Pizza> pizza = nullptr;
    size_t nbPizzas = 0;
    VecStr_t tokens = split(order, ' ');

    if (!this->orderIsCorrect(tokens)) {
        std::cout << "Invalid order." << std::endl;
        return false;
    }
    pizza = std::make_shared<Pizza>(getPizzaType(tokens[0]),
                                    getPizzaSize(tokens[1]));
    nbPizzas = getNumber<size_t>(tokens[2].substr(1));
    for (size_t i = 0 ; i < nbPizzas ; i++)
        this->delegateOrder(pizza);
    return true;
}

void Reception::delegateOrder(std::shared_ptr<Pizza> pizza) {
    std::string orderStr = getPizzaType(pizza->type) + " "
                            + getPizzaSize(pizza->size);
    std::string response;
    pid_t pid = 0;

    if (this->_msgQueues.size() == 0)
        this->createKitchen();
    for (auto &[pid, msgQueue] : this->_msgQueues) {
        msgQueue->send(ASK_ORDER, orderStr);
        response = msgQueue->recv(ORDER);
        if (response == "Accepted") {
            std::cout << "Order placed for " << *pizza << "." << std::endl;
            break;
        }
    }
    if (response == "Refused") {
        pid = this->createKitchen();
        this->_msgQueues[pid]->send(ASK_ORDER, orderStr);
        response = this->_msgQueues[pid]->recv(ORDER);
        if (response == "Accepted")
            std::cout << "Order placed for " << *pizza << "." << std::endl;
        else
            std::cout << "Failed to place " << *pizza << "." << std::endl;
    }
}

bool Reception::orderIsCorrect(const VecStr_t &tokens) {
    const char *nbPizzas = NULL;

    if (tokens.size() != 3) {
        std::cerr << "Pizza ordering must be: TYPE SIZE NUMBER." << std::endl;
        return false;
    }
    try {
        getPizzaType(tokens[0]);
        getPizzaSize(tokens[1]);
    } catch (const PizzaError &err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    nbPizzas = tokens[2].c_str() + 1;
    if (!isPositiveNumber<size_t>(nbPizzas) || !getNumber<size_t>(nbPizzas)) {
        std::cerr << "The number of pizzas must be positive." << std::endl;
        return false;
    }
    return true;
}


void Reception::handleReceived(void) {
    std::string message;
    bool disconnected = false;

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        message = msgQueue->recv(PIZZA, MSG_NOERROR | IPC_NOWAIT);
        if (!message.empty())
            std::cout << "Finished making " << message << "." << std::endl;
        message = msgQueue->recv(DISCONNECTION, MSG_NOERROR | IPC_NOWAIT);
        if (!message.empty()) {
            std::cout << "Disconnection from Kitchen " << pid << std::endl;
            disconnected = true;
            this->_msgQueues.erase(pid);
            break;
        }
    }
    if (disconnected)
        this->handleReceived();
}

pid_t Reception::createKitchen(void) {
    auto msgQueue = std::make_shared<MessageQueue>(
        MAIN_FILE_PATH, _msgQueues.size());
    pid_t pid = fork();
    std::string message;

    if (pid == -1) {
        throw ReceptionError();
    } else if (pid == 0) {
        plz::Kitchen kitchen(this->_kitchenSettings, msgQueue);
        kitchen.run();
        _exit(0);
    }
    _msgQueues[pid] = msgQueue;
    std::this_thread::sleep_for(50ms);
    if (msgQueue->recv(CONNECTION) == "Connected")
        std::cerr << "Created new kitchen." << std::endl;
    else
        std::cerr << "Failed to create new kitchen." << std::endl;
    return pid;
}
}  // namespace plz
