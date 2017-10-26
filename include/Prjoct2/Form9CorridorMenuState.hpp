#ifndef FORM9_CORRIDOR_MENU_STATE_HPP
#define FORM9_CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class Form9CorridorMenuState : public MenuState {
	sf::FloatRect m_door;

public:
	typedef std::shared_ptr<Form9CorridorMenuState> Ptr;

	Form9CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM9_CORRIDOR_MENU_STATE_HPP

