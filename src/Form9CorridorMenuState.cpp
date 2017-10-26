#include<Prjoct2/Form9CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/App.hpp>

Form9CorridorMenuState::Form9CorridorMenuState()
	: MenuState("9formCorridorMenu")
	, m_door(415, 170, 550, 530) {
}

void Form9CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "9formCorridorMenuDoorHover") {
			App::get().m_states.push(std::make_shared
				<EditorState>());
		}
		break;
	}
}

void Form9CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_door)) {
		m_data = "9formCorridorMenuDoorHover";
	} else {
		m_data = "9formCorridorMenu";
	}
}

void Form9CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

