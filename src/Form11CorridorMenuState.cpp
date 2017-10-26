#include<Prjoct2/Form11CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/App.hpp>

Form11CorridorMenuState::Form11CorridorMenuState()
	: MenuState("11formCorridorMenu")
	, m_door(175, 170, 310, 530) {
}

void Form11CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "11formCorridorMenuDoorHover") {
			App::get().m_states.push(std::make_shared
				<EditorState>());
		}
		break;
	}
}

void Form11CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_door)) {
		m_data = "11formCorridorMenuDoorHover";
	} else {
		m_data = "11formCorridorMenu";
	}
}

void Form11CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

