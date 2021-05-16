/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** utils
*/

#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
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

static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return ch != ' ' && ch != '\t';
    }));
}

static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return ch != ' ' && ch != '\t';
    }).base(), s.end());
}

void trim(std::string &str) {
    ltrim(str);
    rtrim(str);
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

std::vector<std::string> getRandomNames(void) {
    std::ifstream file(COOK_NAMES_PATH);
    std::vector<std::string> names;
    std::string name;

    if (!file.is_open()) {
        std::cerr << "Couldn't open " << COOK_NAMES_PATH << std::endl;
        return names;
    }
    while (getline(file, name))
        names.push_back(name);
    return names;
}
