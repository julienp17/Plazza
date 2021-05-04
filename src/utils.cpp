/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** utils
*/

#include <vector>
#include <random>
#include "utils.hpp"

VecStr_t split(const std::string &str, char delim) {
    VecStr_t tokens;
    std::istringstream iss(str);
    std::string item;

    while (std::getline(iss, item, delim))
        if (!item.empty())
            tokens.push_back(item);
    return tokens;
}

std::chrono::milliseconds getElapsedTime(
    std::chrono::time_point<std::chrono::steady_clock> t1) {
    auto t2 = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
}

void resetTimepoint(
        std::chrono::time_point<std::chrono::steady_clock> &timepoint) {
    timepoint = std::chrono::steady_clock::now();
}

std::string getRandomName(void) {
    std::vector<std::string> names = {
        "Lolo", "Paul", "Laura", "Estellia", "Dorian", "Julien", "Tristan",
        "Antoine", "Anthony", "William", "Kevin", "Clément", "Thomas", "Dylan",
        "Laurent", "Didier", "Robert", "Samuel", "Etienne", "Luan", "Jules",
        "Matéo", "Mathias", "Jackie", "Michel", "Marc", "Marie", "Carlos",
        "Juan", "Fred", "Teddy", "Salt Bae", "Brice", "Philippe", "Etchebest",
        "Gordon", "Paolo", "Andrea", "Johnny", "Cyril", "Itodoki", "Bilel",
        "Jarry", "Li", "Mohamed", "Marcus", "Abdul", "Michael", "Francisco"
    };

    return names[rand() % names.size()];
}