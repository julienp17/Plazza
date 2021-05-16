/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "Reception.hpp"
#include "Error.hpp"
#include "utils.hpp"
#include "Logger.hpp"

namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */

Reception::Reception(void) {
    initLogger("./plazza.log", ldebug);
    this->_orderID = 1;
}

Reception::~Reception(void) {
    endLogger();
}

/* ---------------------------- Member functions ---------------------------- */

void Reception::close(void) {
    std::cout << "Closing the restaurant, please wait..." << std::endl;
    for (auto &[pid, msgQueue] : this->_msgQueues) {
        waitpid(pid, NULL, 0);
        msgQueue->close();
    }
    FILE_LOG(linfo) << "Restaurant is closed.";
}

std::string Reception::getStatus(const pid_t pid) {
    std::string status;

    if (this->_msgQueues.find(pid) == this->_msgQueues.end()) {
        return "";
    }
    this->_msgQueues[pid]->send(ASK_STATUS, "Status");
    std::this_thread::sleep_for(50ms);
    return this->_msgQueues[pid]->recv(STATUS);
}

std::vector<pid_t> Reception::getKitchenPIDs(void) const {
    std::vector<pid_t> pids;

    for (auto &[pid, _] : this->_msgQueues) {
        pids.push_back(pid);
    }
    return pids;
}

bool Reception::placeOrder(const std::string &orderFmt) {
    size_t nbPizzas = 0;
    VecStr_t tokens = split(orderFmt, ' ');
    std::stringstream order;

    if (!this->orderIsCorrect(tokens)) {
        std::cout << "Invalid order." << std::endl;
        return false;
    }
    nbPizzas = getNumber<size_t>(tokens[2].substr(1));
    for (size_t i = 0 ; i < nbPizzas ; i++) {
        order.str("");
        order.clear();
        order << tokens[0] << " " << tokens[1] << " "
            << std::setfill('0') << std::setw(3) << this->_orderID;
        this->_orderID++;
        this->delegateOrder(order.str());
    }
    return true;
}

void Reception::delegateOrder(const std::string &order) {
    bool sent = false;
    pid_t nPid = 0;

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        sent = this->sendOrder(msgQueue, order);
        std::this_thread::sleep_for(50ms);
        if (!sent)
            continue;
        std::cout << "Order placed for " << order << "." << std::endl;
        FILE_LOG(linfo) << "Order [" << order << "] sent to kitchen " << pid;
        break;
    }
    if (!sent) {
        nPid = this->createKitchen();
        sent = this->sendOrder(this->_msgQueues[nPid], order);
        std::this_thread::sleep_for(50ms);
        if (!sent) {
            std::cout << "Failed to place " << order << "." << std::endl;
            return;
        }
        std::cout << "Order placed for " << order << "." << std::endl;
        FILE_LOG(linfo) << "Order [" << order << "] sent to kitchen " << nPid;
    }
}

bool Reception::sendOrder(std::shared_ptr<MessageQueue> msgQueue,
                            const std::string &orderStr) {
    msgQueue->send(ASK_ORDER, orderStr);
    std::this_thread::sleep_for(50ms);
    return msgQueue->recv(ORDER) == "Accepted";
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

std::string Reception::handleReceived(void) {
    std::string message;

    for (auto &[pid, msgQueue] : this->_msgQueues) {
        message = msgQueue->recv(PIZZA, MSG_NOERROR | IPC_NOWAIT);
        if (!message.empty()) {
            std::cout << "Finished making " << message << "." << std::endl;
            FILE_LOG(linfo) << message << " delivered.";
            return message;
        }
    }
    return message;
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
        srand(time(NULL) + _msgQueues.size());
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
