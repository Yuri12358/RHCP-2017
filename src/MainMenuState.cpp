#include<Prjoct2/CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/MainMenuState.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

MainMenuState::MainMenuState()
	: MenuState("mainMenu") {
}

void MainMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_window.close();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "mainMenuButtonHover") {
			App::get().m_states.push(std::make_shared
				<CorridorMenuState>());
		}
		break;
	}
}

void MainMenuState::update() {
	if (m_checkTexturePartHover(220, 440, 360, 550)) {
		m_data = "mainMenuButtonHover";
	} else {
		m_data = "mainMenu";
	}
}

void MainMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

