#include<Prjoct2/CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/App.hpp>

CorridorMenuState::CorridorMenuState()
	: MenuState("") {
}

void CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data != "") {
			App::get().m_states.push(std::make_shared
				<EditorState>());
		}
		break;
	}
}

void CorridorMenuState::update() {
	if (m_checkTexturePartHover(260, 500, 400, 535)) {
		m_data = "5";
	} else if (m_checkTexturePartHover(295, 395, 360, 415)) {
		m_data = "9";
	} else if (m_checkTexturePartHover(305, 355, 350, 370)) {
		m_data = "11";
	} else {
		m_data = "";
	}
}

void CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["corridorMenu"], true);
	App::get().window().draw(bg);
}

