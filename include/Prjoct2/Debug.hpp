#ifndef DEBUG_HPP
#define DEBUG_HPP

#include<nlohmann/json.hpp>

void print(nlohmann::json & json);
nlohmann::json * getParent(nlohmann::json & pin);
void printField();

#endif // DEBUG_HPP

