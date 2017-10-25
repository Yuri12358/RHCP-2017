#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include<Prjoct2/State.hpp>

class MainMenuState : public State {
	std::string m_bgTextureName;

public:
	typedef std::shared_ptr<MainMenuState> Ptr;

	MainMenuState();
	void handleEvent(sf::Event event);
	void render();
	void update();
};

#endif // MENU_STATE_HPP

