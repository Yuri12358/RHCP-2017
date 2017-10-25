#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/App.hpp>
#include<iostream>
#include<iomanip>

App * App::s_instance = nullptr;

App::App()
	: m_window(sf::VideoMode(
		JSONHolder::get()["settings"]["window"]["width"].get<int>(),
		JSONHolder::get()["settings"]["window"]["height"].get<int>()),
		JSONHolder::get()["settings"]["window"]["title"]
			.get<std::string>(),
		JSONHolder::get()["settings"]["window"]["fullscreen"]
			.get<bool>() ? sf::Style::Fullscreen 
			: sf::Style::Default)
	, m_defaultView(m_window.getDefaultView()) {
	m_window.clear();
	m_window.display();
	JSONHolder::get()["next component type"] = "dot";
}

App & App::get() {
	if (s_instance == nullptr) {
		s_instance = new App();
	}
	return *s_instance;
}

void App::run() {
	m_state = std::make_shared<MainMenuState>();
	while (m_window.isOpen()) {
		m_handleEvents();
		m_render();
		m_state->update();
	}
}

void App::m_handleEvents() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		try {
			if (GUIHolder::get().gui().handleEvent(event) 
				&& event.type != sf::Event::KeyPressed
				|| editor().isUILocked() && event.type
				!= sf::Event::Closed) {
				continue;
			}
		} catch (std::logic_error e) {}
		switch (event.type) {
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::Resized:
			m_handleResizeEvent(event.size);
			break;
		default:
			m_state->handleEvent(event);
			break;
		}
	}
}

void App::m_handleResizeEvent(const sf::Event::SizeEvent & event) {
	m_defaultView.reset(sf::FloatRect(0, 0, event.width, event.height));
	try {
		editor().updateFieldView();
	} catch (std::logic_error e) {}
	m_window.setView(m_defaultView);
	GUIHolder::get().gui().setView(m_defaultView);
}

void App::m_render() {
	m_window.clear(sf::Color::White);
	m_state->render();
	GUIHolder::get().gui().draw();
	m_window.display();
}

sf::RenderWindow & App::window() {
	return m_window;
}

EditorState & App::editor() {
	EditorState::Ptr editor = std::dynamic_pointer_cast<EditorState>
		(m_state);
	if (editor) {
		return *editor;
	}
	throw std::logic_error(
		"Attempt to get editor state that doesn't exist");
}

