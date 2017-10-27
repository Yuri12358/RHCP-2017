#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class MainMenuState : public MenuState {
	sf::FloatRect m_arrow;
public:
	typedef std::shared_ptr<MainMenuState> Ptr;

	MainMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // MAIN_MENU_STATE_HPP

