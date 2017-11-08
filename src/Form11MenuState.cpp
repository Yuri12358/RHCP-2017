#include<Prjoct2/Form11MenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

Form11CorridorMenuState::Form11CorridorMenuState()
	: MenuState("corridorMenu")
	, m_door(175, 170, 310, 530) {
	GUIHolder::get().setResourcePack("11");
}

void Form11CorridorMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			GUIHolder::get().setResourcePack();
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "corridorMenuDoorHover") {
			App::get().enterState(std::make_shared
				<Form11ClassMenuState>());
		}
		break;
	}
}

void Form11CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_door)) {
		m_data = "corridorMenuDoorHover";
	} else {
		m_data = "corridorMenu";
	}
}

void Form11CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

Form11ClassMenuState::Form11ClassMenuState()
	: MenuState("classMenu") {
	m_arrows["desk"] = sf::FloatRect(490, 460, 595, 485);
	m_arrows["back"] = sf::FloatRect(380, 480, 430, 550);
}

void Form11ClassMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "desk") {
			App::get().enterState(std::make_shared
				<Form11DeskMenuState>());
		} else if (m_data == "back") {
			App::get().leaveState();
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
	bg.setTexture(&TextureHolder::get()["classMenu"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().window().getSize());
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
	App::get().window().draw(arrow);
}

Form11DeskMenuState::Form11DeskMenuState()
	: MenuState("deskMenu") {
	m_books["reference"] = sf::FloatRect(260, 330, 320, 390);
	m_books["editor"] = sf::FloatRect(400, 330, 460, 400);
	m_books["zno"] = sf::FloatRect(480, 325, 540, 405);
	m_books["exercises"] = sf::FloatRect(550, 340, 625, 405);
}

void Form11DeskMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "editor") {
			App::get().enterState(std::make_shared
				<EditorState>());
		} else if (m_data == "reference") {
			App::get().enterState(std::make_shared
				<Form11ReferenceState>());
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
	bg.setTexture(&TextureHolder::get()["deskMenu"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().window().getSize());
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
	App::get().window().draw(arrow);
}

Form11ReferenceState::Form11ReferenceState()
	: MenuState("deskMenu")
	, m_button(755, 0, 785, 35) {
}

void Form11ReferenceState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "close") {
			App::get().leaveState();
		}
		break;
	}
}

void Form11ReferenceState::update() {
	if (m_checkTexturePartHover(m_button)) {
		m_data = "close";
	} else {
		m_data = "";
	}
}

void Form11ReferenceState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()["reference"], true);
	App::get().window().draw(bg);
	if (m_data == "") {
		return;
	}
	sf::Vector2f winSize(App::get().window().getSize());
	sf::Vector2f bgSize(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_button.left, m_button.top), bgSize), winSize));
	sf::Vector2f drCorner(pairwiseMultiply(pairwiseDivide(sf::Vector2f(
		m_button.width, m_button.height), bgSize), winSize));
	sf::RectangleShape button(drCorner - ulCorner);
	button.setPosition(ulCorner);
	button.setFillColor(sf::Color(255, 255, 255, 128));
	App::get().window().draw(button);
}

