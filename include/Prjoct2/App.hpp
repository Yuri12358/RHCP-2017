#ifndef APP_HPP
#define APP_HPP

#include"Precomp.h"

#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/State.hpp>
#include<SFML/Graphics.hpp>
#include<stack>

class App {
	App();
	void m_handleEvents();
	void m_handleResizeEvent(const sf::Event::SizeEvent & event);
	void m_render();

	static App * s_instance;
	sf::RenderWindow m_window;
	sf::View m_defaultView;
	std::stack<State::Ptr> m_states;

public:
	static App & get();
	void run();
	sf::RenderWindow & window();
	const sf::View & defaultView();
	EditorState & editor();
	void enterState(State::Ptr state);
	void setState(State::Ptr state);
	void leaveState();
};

#endif // APP_HPP

