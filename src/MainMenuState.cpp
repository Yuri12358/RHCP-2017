#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/MainMenuState.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

MainMenuState::MainMenuState()
	: m_bgTextureName("mainMenu") {
	GUIHolder::get().clear();
}

void MainMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_window.close();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_bgTextureName == "mainMenuButtonHover") {
			App::get().m_state = std::make_shared<EditorState>();
		}
		break;
	}
}

void MainMenuState::update() {
	sf::Vector2f size(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(220, 440);
	sf::Vector2f drCorner(360, 550);
	sf::FloatRect normalButtonRect(pairwiseDivide(ulCorner, size),
		pairwiseDivide(drCorner - ulCorner, size));
	sf::Vector2f normalMouse(pairwiseDivide(sf::Vector2f(sf::Mouse
		::getPosition(App::get().m_window)), sf::Vector2f(App::get()
		.m_window.getSize())));
	if (normalButtonRect.contains(normalMouse)) {
		m_bgTextureName = "mainMenuButtonHover";
	} else {
		m_bgTextureName = "mainMenu";
	}
}

void MainMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().m_window.getSize()));
	bg.setTexture(&TextureHolder::get()[m_bgTextureName], true);
	App::get().m_window.draw(bg);
}

