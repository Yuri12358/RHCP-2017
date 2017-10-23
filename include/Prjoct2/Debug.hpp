#ifndef DEBUG_HPP
#define DEBUG_HPP

#include<Prjoct2/Precomp.h>

void print(nlohmann::json & json);
nlohmann::json * getParent(nlohmann::json & pin);
void printField();

#endif // DEBUG_HPP

