#ifndef CORRIDOR_MENU_STATE_HPP
#define CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class CorridorMenuState : public MenuState {
public:
	typedef std::shared_ptr<CorridorMenuState> Ptr;

	CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // CORRIDOR_MENU_STATE_HPP

