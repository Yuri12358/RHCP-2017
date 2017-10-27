#include<Prjoct2/Form5MenuState.hpp>
#include<Prjoct2/Form9MenuState.hpp>
#include<Prjoct2/Form11MenuState.hpp>
#include<Prjoct2/CorridorMenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

CorridorMenuState::CorridorMenuState()
	: MenuState("") {
	m_arrows["5"] = sf::FloatRect(260, 500, 400, 535);
	m_arrows["9"] = sf::FloatRect(295, 395, 360, 415);
	m_arrows["11"] = sf::FloatRect(305, 355, 350, 370);
}

void CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "5") {
			App::get().m_states.push(std::make_shared
				<Form5CorridorMenuState>());
		} else if (m_data == "9") {
			App::get().m_states.push(std::make_shared
				<Form9CorridorMenuState>());
		} else if (m_data == "11") {
			App::get().m_states.push(std::make_shared
				<Form11CorridorMenuState>());
		}
		break;
	}
}

void CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_arrows["5"])) {
		m_data = "5";
	} else if (m_checkTexturePartHover(m_arrows["9"])) {
		m_data = "9";
	} else if (m_checkTexturePartHover(m_arrows["11"])) {
		m_data = "11";
	} else {
		m_data = "";
	}
}

void CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["corridorMenu"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().m_window.getSize());
	sf::Vector2f bgSize(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(m_arrows[m_data].left, m_arrows[m_data].top);
	sf::Vector2f drCorner(m_arrows[m_data].width, m_arrows[m_data].height);
	sf::Vector2f ulCornerWin(pairwiseMultiply(pairwiseDivide(ulCorner,
		bgSize), winSize));
	sf::Vector2f drCornerWin(pairwiseMultiply(pairwiseDivide(drCorner,
		bgSize), winSize));
	sf::Vector2f size(drCornerWin - ulCornerWin);
	sf::RectangleShape arrow(size);
	arrow.setPosition(ulCornerWin);
	arrow.setFillColor(sf::Color(255, 255, 255, 128));
	App::get().window().draw(arrow);
}

