#include<Prjoct2/Form11MenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/Utility.hpp>
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
				<Form11ClassMenuState>());
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

Form11ClassMenuState::Form11ClassMenuState()
	: MenuState("11formClassMenu") {
	m_arrows["desk"] = sf::FloatRect(490, 460, 595, 485);
	m_arrows["back"] = sf::FloatRect(380, 480, 430, 550);
}

void Form11ClassMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "desk") {
			App::get().m_states.push(std::make_shared
				<Form11DeskMenuState>());
		} else if (m_data == "back") {
			App::get().m_states.pop();
		}
		break;
	}
}

void Form11ClassMenuState::update() {
	if (m_checkTexturePartHover(m_arrows["desk"])) {
		m_data = "desk";
	} else if (m_checkTexturePartHover(m_arrows["back"])) {
		m_data = "back";
	} else {
		m_data = "";
	}
}

void Form11ClassMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["11formClassMenu"], true);
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

Form11DeskMenuState::Form11DeskMenuState()
	: MenuState("11formDeskMenu") {
	m_books["reference"] = sf::FloatRect(260, 330, 320, 390);
	m_books["editor"] = sf::FloatRect(400, 330, 460, 400);
	m_books["zno"] = sf::FloatRect(480, 325, 540, 405);
	m_books["exercises"] = sf::FloatRect(550, 340, 625, 405);
}

void Form11DeskMenuState::handleEvent(sf::Event event) {
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

void Form11DeskMenuState::update() {
	if (m_checkTexturePartHover(m_books["zno"])) {
		m_data = "zno";
	} else if (m_checkTexturePartHover(m_books["editor"])) {
		m_data = "editor";
	} else if (m_checkTexturePartHover(m_books["reference"])) {
		m_data = "reference";
	} else if (m_checkTexturePartHover(m_books["exercises"])) {
		m_data = "exercises";
	} else {
		m_data = "";
	}
}

void Form11DeskMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["11formDeskMenu"], true);
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

