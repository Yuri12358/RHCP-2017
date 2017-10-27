#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include<Prjoct2/State.hpp>
#include<SFML/Graphics.hpp>

class MenuState : public State {
protected:
	std::string m_data;

	bool m_checkTexturePartHover(sf::FloatRect rect);

public:
	typedef std::shared_ptr<MenuState> Ptr;

	MenuState(const std::string & data);
};

#endif // MENU_STATE_HPP

