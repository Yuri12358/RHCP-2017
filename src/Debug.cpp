#include<Prjoct2/Debug.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<iostream>
#include<iomanip>

void print(nlohmann::json & json) {
	std::cout << std::setw(4) << json << std::endl;
}

nlohmann::json * getParent(nlohmann::json & pin) {
	if (pin.count("parentID") == 1) {
		return &JSONHolder::get()["components"]
			[pin["parentID"].get<std::string>()];
	}
	return nullptr;
}

void printField() {
	std::cout << std::setw(4) << JSONHolder::get()["components"]
		<< std::endl;
}

