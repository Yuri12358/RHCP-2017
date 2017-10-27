#include<Prjoct2/Form9MenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/Utility.hpp>
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
				<Form9ClassMenuState>());
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

Form9ClassMenuState::Form9ClassMenuState()
	: MenuState("9formClassMenu") {
	m_arrows["desk"] = sf::FloatRect(420, 440, 530, 470);
	m_arrows["back"] = sf::FloatRect(315, 460, 360, 530);
}

void Form9ClassMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().m_states.pop();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "desk") {
			App::get().m_states.push(std::make_shared
				<Form9DeskMenuState>());
		} else if (m_data == "back") {
			App::get().m_states.pop();
		}
		break;
	}
}

void Form9ClassMenuState::update() {
	if (m_checkTexturePartHover(m_arrows["desk"])) {
		m_data = "desk";
	} else if (m_checkTexturePartHover(m_arrows["back"])) {
		m_data = "back";
	} else {
		m_data = "";
	}
}

void Form9ClassMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["9formClassMenu"], true);
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

Form9DeskMenuState::Form9DeskMenuState()
	: MenuState("9formDeskMenu") {
	m_books["reference"] = sf::FloatRect(270, 285, 330, 350);
	m_books["editor"] = sf::FloatRect(400, 295, 450, 355);
	m_books["labtasks"] = sf::FloatRect(520, 290, 580, 355);
	m_books["exercises"] = sf::FloatRect(585, 295, 640, 360);
	m_books["journal"] = sf::FloatRect(175, 270, 260, 375);
}

void Form9DeskMenuState::handleEvent(sf::Event event) {
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

void Form9DeskMenuState::update() {
	if (m_checkTexturePartHover(m_books["labtasks"])) {
		m_data = "labtasks";
	} else if (m_checkTexturePartHover(m_books["editor"])) {
		m_data = "editor";
	} else if (m_checkTexturePartHover(m_books["reference"])) {
		m_data = "reference";
	} else if (m_checkTexturePartHover(m_books["exercises"])) {
		m_data = "exercises";
	} else if (m_checkTexturePartHover(m_books["journal"])) {
		m_data = "journal";
	} else {
		m_data = "";
	}
}

void Form9DeskMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["9formDeskMenu"], true);
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

