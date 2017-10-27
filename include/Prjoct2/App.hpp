#ifndef APP_HPP
#define APP_HPP

#include"Precomp.h"

#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/State.hpp>
#include<SFML/Graphics.hpp>
#include<stack>

class MainMenuState;
class CorridorMenuState;
class Form5CorridorMenuState;
class Form5ClassMenuState;
class Form5DeskMenuState;
class Form9CorridorMenuState;
class Form9ClassMenuState;
class Form9DeskMenuState;
class Form11CorridorMenuState;
class Form11ClassMenuState;
class Form11DeskMenuState;

class App {
	friend class MainMenuState;
	friend class CorridorMenuState;
	friend class Form5CorridorMenuState;
	friend class Form5ClassMenuState;
	friend class Form5DeskMenuState;
	friend class Form9CorridorMenuState;
	friend class Form9ClassMenuState;
	friend class Form9DeskMenuState;
	friend class Form11CorridorMenuState;
	friend class Form11ClassMenuState;
	friend class Form11DeskMenuState;
	friend class EditorState;

	App();
	void m_handleEvents();
	void m_handleResizeEvent(const sf::Event::SizeEvent & event);
	void m_render();

	static App * s_instance;
	sf::RenderWindow m_window;
	sf::View m_defaultView;
	std::string m_currentCircuitName;
	std::stack<State::Ptr> m_states;

public:
	static App & get();
	void run();
	sf::RenderWindow & window();
	EditorState & editor();
};

#endif // APP_HPP

