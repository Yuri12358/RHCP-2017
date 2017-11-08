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

#include<map>

class Form9ClassMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_arrows;

public:
	typedef std::shared_ptr<Form9ClassMenuState> Ptr;

	Form9ClassMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form9DeskMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_books;

public:
	typedef std::shared_ptr<Form9DeskMenuState> Ptr;

	Form9DeskMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form9ReferenceState : public MenuState {
	sf::FloatRect m_button;

public:
	Form9ReferenceState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form9IntroState : public State {
public:
	typedef std::shared_ptr<Form9IntroState> Ptr;

	Form9IntroState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM9_CORRIDOR_MENU_STATE_HPP

