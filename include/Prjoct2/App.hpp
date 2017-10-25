#ifndef APP_HPP
#define APP_HPP

#include"Precomp.h"

#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/MainMenuState.hpp>

class App {
	friend class EditorState;
	friend class MainMenuState;

	App();
	void m_handleEvents();
	void m_handleResizeEvent(const sf::Event::SizeEvent & event);
	void m_render();

	static App * s_instance;
	sf::RenderWindow m_window;
	sf::View m_defaultView;
	std::string m_currentCircuitName;
	State::Ptr m_state;

public:
	static App & get();
	void run();
	sf::RenderWindow & window();
	EditorState & editor();
};

#endif // APP_HPP

