#include<Prjoct2/Form5CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/App.hpp>

Form5CorridorMenuState::Form5CorridorMenuState()
	: MenuState("5formCorridorMenu")
	, m_door(445, 170, 580, 530) {
}

void Form5CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "5formCorridorMenuDoorHover") {
			App::get().m_states.push(std::make_shared
				<EditorState>());
		}
		break;
	}
}

void Form5CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_door)) {
		m_data = "5formCorridorMenuDoorHover";
	} else {
		m_data = "5formCorridorMenu";
	}
}

void Form5CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

