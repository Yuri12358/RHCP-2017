#ifndef COMPONENT_INFO_HPP
#define COMPONENT_INFO_HPP

#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>

class ComponentInfo {
public:
	static sf::IntRect getRect(const std::string & id);
	static sf::IntRect getRect(const nlohmann::json & component);
	static nlohmann::json * getComponentPin(const std::string & id,
		int pinX, int pinY);
	static nlohmann::json * getComponentPinAt(const std::string & id,
		int x, int y);
};

#endif // COMPONENT_INFO_HPP

