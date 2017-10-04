#ifndef COMPONENT_INFO_HPP
#define COMPONENT_INFO_HPP

#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>

enum Direction {
	None		= 0,
	Up		= (1 << 0),
	Right		= (1 << 1),
	Down		= (1 << 2),
	Left		= (1 << 3),
	UpRight		= (Up | Right),
	UpLeft		= (Up | Left),
	DownRight	= (Down | Right),
	DownLeft	= (Down | Left),
	All		= (Up | Right | Down | Left)
};

class ComponentInfo {
public:
	static sf::IntRect getRect(const std::string & id);
	static sf::IntRect getRect(const nlohmann::json & component);
	static nlohmann::json * getComponentPin(const std::string & id,
		int pinX, int pinY);
	static nlohmann::json * getComponentPinAt(const std::string & id,
		int x, int y);
	static Direction getPinOrientation(const nlohmann::json & pin);
};

#endif // COMPONENT_INFO_HPP

