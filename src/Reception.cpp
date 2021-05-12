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
#include "Logger.hpp"

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

Reception::Reception(void) {
    initLogger("plazza.log", ldebug);
}

Reception::~Reception(void) {
    for (auto &[pid, msgQueue] : this->_msgQueues) {
        msgQueue->close();
        waitpid(pid, NULL, 0);
    }
    endLogger();
}

/* ---------------------------- Member functions ---------------------------- */

void Reception::run(void) {
    std::string commands;

    setSigIntHandler();
    FILE_LOG(linfo) << "Restaurant is open.";
    FILE_LOG(linfo) << _kitchenSettings;
    std::cout << "Welcome to Plazza! The Pizzeria for everyone!" << std::endl;
    while (isOpen) {
        std::cout << "Place your order here: ";
        getline(std::cin, commands);
        this->doCommands(commands);
    }
    std::cout << "Closing the restaurant..." << std::endl;
    FILE_LOG(linfo) << "Restaurant is closed.";
}

void Reception::doCommands(const std::string &commands_str) {
    VecStr_t commands;

    commands = split(commands_str, ';');
    for (std::string &command : commands) {
        trim(command);
        FILE_LOG(linfo) << "Received command : [" << command << "]";
        this->handleReceived();
        this->handleDisconnections();
        if (strcasecmp(command.c_str(), "status") == 0)
            this->printStatus();
        else if (strcasecmp(command.c_str(), "exit") == 0)
            isOpen = false;
        else
            this->placeOrder(command);
    }
}

void Reception::printStatus(void) {
    for (auto &[pid, msgQueue] : this->_msgQueues) {
        msgQueue->send(ASK_STATUS, "Status");
        std::cout << "Kitchen " << pid << ":" << std::endl;
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
    std::string response = "Refused";
    pid_t newPid = 0;

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        msgQueue->send(ASK_ORDER, orderStr);
        response = msgQueue->recv(ORDER);
        if (response == "Accepted") {
            std::cout << "Order placed for " << *pizza << "." << std::endl;
            FILE_LOG(linfo) << "Order [" << *pizza << "] sent to kitchen " << pid;
            break;
        }
    }
    if (response == "Refused") {
        newPid = this->createKitchen();
        this->_msgQueues[newPid]->send(ASK_ORDER, orderStr);
        response = this->_msgQueues[newPid]->recv(ORDER);
        if (response == "Accepted") {
            std::cout << "Order placed for " << *pizza << "." << std::endl;
            FILE_LOG(linfo) << "Order [" << *pizza << "] sent to kitchen " << newPid;
        } else {
            std::cout << "Failed to place " << *pizza << "." << std::endl;
        }
    }
}

bool Reception::orderIsCorrect(const VecStr_t &tokens) {
    const char *nbPizzas = NULL;

    if (tokens.size() != 3) {
        FILE_LOG(ldebug) << "Pizza ordering must be: TYPE SIZE NUMBER.";
        return false;
    }
    try {
        getPizzaType(tokens[0]);
        getPizzaSize(tokens[1]);
    } catch (const PizzaError &err) {
        FILE_LOG(ldebug) << err.what();
        return false;
    }
    nbPizzas = tokens[2].c_str() + 1;
    if (!isPositiveNumber<size_t>(nbPizzas) || !getNumber<size_t>(nbPizzas)) {
        FILE_LOG(ldebug) << "The number of pizzas must be positive.";
        return false;
    }
    return true;
}

void Reception::handleReceived(void) {
    std::string message = "hey";

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        while (!message.empty()) {
            message = msgQueue->recv(PIZZA, MSG_NOERROR | IPC_NOWAIT);
            if (!message.empty()) {
                std::cout << "Finished making " << message << "." << std::endl;
                FILE_LOG(linfo) << message << " delivered.";
            }
        }
        message = "hey";
    }
}

void Reception::handleDisconnections(void) {
    std::string message;
    bool disconnected = false;

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        message = msgQueue->recv(DISCONNECTION, MSG_NOERROR | IPC_NOWAIT);
        if (!message.empty()) {
            msgQueue->close();
            disconnected = true;
            this->_msgQueues.erase(pid);
            break;
        }
    }
    if (disconnected)
        this->handleDisconnections();
}

pid_t Reception::createKitchen(void) {
    auto msgQueue = std::make_shared<MessageQueue>(
        MAIN_FILE_PATH, _msgQueues.size());
    pid_t pid = fork();
    std::string message;

    if (pid == -1) {
        throw ReceptionError();
    } else if (pid == 0) {
        srand(time(NULL));
        plz::Kitchen kitchen(this->_kitchenSettings, msgQueue);
        kitchen.run();
        _exit(0);
    }
    _msgQueues[pid] = msgQueue;
    std::this_thread::sleep_for(50ms);
    if (msgQueue->recv(CONNECTION) == "Connected")
        FILE_LOG(linfo) << "Kitchen " << pid << " opened.";
    else
        FILE_LOG(lerror) << "Failed to create new kitchen.";
    return pid;
}
}  // namespace plz
