#include<Prjoct2/Form5MenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/Utility.hpp>
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
				<Form5ClassMenuState>());
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

Form5ClassMenuState::Form5ClassMenuState()
	: MenuState("5formClassMenu") {
	m_arrows["desk"] = sf::FloatRect(470, 465, 550, 495);
	m_arrows["back"] = sf::FloatRect(570, 480, 630, 550);
}

void Form5ClassMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "desk") {
			App::get().m_states.push(std::make_shared
				<Form5DeskMenuState>());
		} else if (m_data == "back") {
			App::get().m_states.pop();
		}
		break;
	}
}

void Form5ClassMenuState::update() {
	if (m_checkTexturePartHover(m_arrows["desk"])) {
		m_data = "desk";
	} else if (m_checkTexturePartHover(m_arrows["back"])) {
		m_data = "back";
	} else {
		m_data = "";
	}
}

void Form5ClassMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["5formClassMenu"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().m_window.getSize());
	sf::Vector2f bgSize(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_arrows[m_data].left, m_arrows[m_data].top), bgSize),
		winSize));
	sf::Vector2f drCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_arrows[m_data].width, m_arrows[m_data].height), bgSize),
		winSize));
	sf::RectangleShape arrow(drCorner - ulCorner);
	arrow.setPosition(ulCorner);
	arrow.setFillColor(sf::Color(255, 255, 255, 128));
	App::get().m_window.draw(arrow);
}

Form5DeskMenuState::Form5DeskMenuState()
	: MenuState("5formDeskMenu") {
	m_books["reference"] = sf::FloatRect(285, 290, 335, 355);
	m_books["editor"] = sf::FloatRect(390, 285, 440, 350);
	m_books["exercises"] = sf::FloatRect(500, 275, 560, 340);
}

void Form5DeskMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "editor") {
			App::get().m_states.push(std::make_shared
				<EditorState>());
		}
		break;
	}
}

void Form5DeskMenuState::update() {
	if (m_checkTexturePartHover(m_books["editor"])) {
		m_data = "editor";
	} else if (m_checkTexturePartHover(m_books["reference"])) {
		m_data = "reference";
	} else if (m_checkTexturePartHover(m_books["exercises"])) {
		m_data = "exercises";
	} else {
		m_data = "";
	}
}

void Form5DeskMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["5formDeskMenu"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().m_window.getSize());
	sf::Vector2f bgSize(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_books[m_data].left, m_books[m_data].top), bgSize),
		winSize));
	sf::Vector2f drCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_books[m_data].width, m_books[m_data].height), bgSize),
		winSize));
	sf::RectangleShape arrow(drCorner - ulCorner);
	arrow.setPosition(ulCorner);
	arrow.setFillColor(sf::Color(255, 255, 255, 128));
	App::get().m_window.draw(arrow);
}

