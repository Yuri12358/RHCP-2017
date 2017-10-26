#ifndef FORM5_CORRIDOR_MENU_STATE_HPP
#define FORM5_CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class Form5CorridorMenuState : public MenuState {
	sf::FloatRect m_door;

public:
	typedef std::shared_ptr<Form5CorridorMenuState> Ptr;

	Form5CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM5_CORRIDOR_MENU_STATE_HPP

