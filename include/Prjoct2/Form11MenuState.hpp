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

#include<map>

class Form11ClassMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_arrows;

public:
	typedef std::shared_ptr<Form11ClassMenuState> Ptr;

	Form11ClassMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form11DeskMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_books;

public:
	typedef std::shared_ptr<Form11DeskMenuState> Ptr;

	Form11DeskMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM11_CORRIDOR_MENU_STATE_HPP

