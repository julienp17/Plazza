/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** MessageQueue
*/

#include <iostream>
#include <stdexcept>
#include <string.h>
#include "MessageQueue.hpp"

MessageQueue::MessageQueue(const char *pathname, int proj_id) {
    key_t key = ftok(pathname, proj_id);

    if (key == -1)
        throw std::runtime_error(strerror(errno));
    _qid = msgget(key, 0666 | IPC_CREAT);
    if (_qid == -1)
        throw std::runtime_error(strerror(errno));
}

MessageQueue::~MessageQueue(void) {
    if (_qid != -1)
        msgctl(_qid, IPC_RMID, NULL);
}

void MessageQueue::send(long type, const std::string &str, int flags) {
    struct msgbuf msg;

    memset(&msg, 0, sizeof(msgbuf));
    msg.mtype = type;
    strncpy(msg.mtext, str.c_str(), MAX_MSG_LEN);
    if (msgsnd(_qid,  &msg, sizeof(msg.mtext), flags) == -1)
        throw std::runtime_error(strerror(errno));
}

std::string MessageQueue::recv(long type, int flags) {
    struct msgbuf msg;

    if (msgrcv(_qid, &msg, sizeof(msg.mtext), type, flags) == -1) {
        if (flags & IPC_NOWAIT && errno != ENOMSG)
            throw std::runtime_error(strerror(errno));
        return std::string();
    }
    return std::string(msg.mtext);
}

void MessageQueue::setId(qid_t qid) {
    if (this->_qid != -1)
        msgctl(_qid, IPC_RMID, NULL);
    this->_qid = qid;
}
