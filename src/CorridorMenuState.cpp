#include<Prjoct2/CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/MainMenuState.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/App.hpp>

CorridorMenuState::CorridorMenuState() {
	GUIHolder::get().clear();
}

void CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_state = std::make_shared<MainMenuState>();
		}
		break;
	case sf::Event::MouseButtonPressed:
		App::get().m_state = std::make_shared<EditorState>();
		break;
	}
}

void CorridorMenuState::update() {
}

void CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().m_window.getSize()));
	bg.setTexture(&TextureHolder::get()["corridorMenu"], true);
	App::get().m_window.draw(bg);
}

