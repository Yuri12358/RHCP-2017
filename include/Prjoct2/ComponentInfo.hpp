#ifndef COMPONENT_INFO_HPP
#define COMPONENT_INFO_HPP

#include"Precomp.h"

#include<Prjoct2/Direction.hpp>

class ComponentInfo {
public:
	static sf::IntRect getRect(const std::string & id);
	static sf::IntRect getRect(const nlohmann::json & component);
	static nlohmann::json * getComponentPin(const std::string & id,
		int pinX, int pinY, bool withRotation = false);
	static nlohmann::json * getComponentPinAt(const std::string & id,
		int x, int y);
	static Direction getPinOrientation(const nlohmann::json & pin);
	static sf::Vector2i rotatePin(const nlohmann::json & pin);
};

#endif // COMPONENT_INFO_HPP

