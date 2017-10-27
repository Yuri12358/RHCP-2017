#ifndef CORRIDOR_MENU_STATE_HPP
#define CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>
#include<map>

class CorridorMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_arrows;
	
public:
	typedef std::shared_ptr<CorridorMenuState> Ptr;

	CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // CORRIDOR_MENU_STATE_HPP

