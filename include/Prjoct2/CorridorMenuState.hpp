#ifndef CORRIDOR_MENU_STATE_HPP
#define CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/State.hpp>

class CorridorMenuState : public State {
public:
	typedef std::shared_ptr<CorridorMenuState> Ptr;

	CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // CORRIDOR_MENU_STATE_HPP

