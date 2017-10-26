#ifndef FORM11_CORRIDOR_MENU_STATE_HPP
#define FORM11_CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class Form11CorridorMenuState : public MenuState {
	sf::FloatRect m_door;

public:
	typedef std::shared_ptr<Form11CorridorMenuState> Ptr;

	Form11CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM11_CORRIDOR_MENU_STATE_HPP

