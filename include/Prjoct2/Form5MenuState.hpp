#ifndef FORM5_CORRIDOR_MENU_STATE_HPP
#define FORM5_CORRIDOR_MENU_STATE_HPP

#include<Prjoct2/MenuState.hpp>

class Form5CorridorMenuState : public MenuState {
	sf::FloatRect m_door;

public:
	typedef std::shared_ptr<Form5CorridorMenuState> Ptr;

	Form5CorridorMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#include<map>

class Form5ClassMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_arrows;

public:
	typedef std::shared_ptr<Form5ClassMenuState> Ptr;

	Form5ClassMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form5DeskMenuState : public MenuState {
	std::map<std::string, sf::FloatRect> m_books;

public:
	typedef std::shared_ptr<Form5DeskMenuState> Ptr;

	Form5DeskMenuState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form5IntroState : public MenuState {
	std::map<std::string, sf::FloatRect> m_buttons;

public:
	typedef std::shared_ptr<Form5IntroState> Ptr;

	Form5IntroState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form5ChooseGenderState : public MenuState {
	std::map<std::string, sf::FloatRect> m_buttons;

public:
	typedef std::shared_ptr<Form5ChooseGenderState> Ptr;

	Form5ChooseGenderState();
	void handleEvent(sf::Event event);
	void update();
	void render();
};

class Form5ReferenceState : public MenuState {
	sf::FloatRect m_button;
	std::string m_bgTextureName;

public:
	Form5ReferenceState(const std::string & bg);
	void handleEvent(sf::Event event);
	void update();
	void render();
};

#endif // FORM5_CORRIDOR_MENU_STATE_HPP

