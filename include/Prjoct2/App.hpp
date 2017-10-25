#ifndef APP_HPP
#define APP_HPP

#include"Precomp.h"

#include<Prjoct2/CorridorMenuState.hpp>
#include<Prjoct2/MainMenuState.hpp>
#include<Prjoct2/EditorState.hpp>
#include<stack>

class App {
	friend class CorridorMenuState;
	friend class MainMenuState;
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

