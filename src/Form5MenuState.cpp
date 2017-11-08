#include<Prjoct2/Form5MenuState.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

Form5CorridorMenuState::Form5CorridorMenuState()
	: MenuState("corridorMenu")
	, m_door(445, 170, 580, 530) {
	GUIHolder::get().setResourcePack("5");
}

void Form5CorridorMenuState::handleEvent(sf::Event event) {
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
				<Form5ClassMenuState>());
		}
		break;
	}
}

void Form5CorridorMenuState::update() {
	if (m_checkTexturePartHover(m_door)) {
		m_data = "corridorMenuDoorHover";
	} else {
		m_data = "corridorMenu";
	}
}

void Form5CorridorMenuState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

Form5ClassMenuState::Form5ClassMenuState()
	: MenuState("classMenu") {
	m_arrows["desk"] = sf::FloatRect(470, 465, 550, 495);
	m_arrows["back"] = sf::FloatRect(570, 480, 630, 550);
}

void Form5ClassMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "desk") {
			App::get().enterState(std::make_shared
				<Form5DeskMenuState>());
		} else if (m_data == "back") {
			App::get().leaveState();
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

Form5DeskMenuState::Form5DeskMenuState()
	: MenuState("deskMenu") {
	m_books["reference"] = sf::FloatRect(285, 290, 335, 355);
	m_books["editor"] = sf::FloatRect(390, 285, 440, 350);
	m_books["exercises"] = sf::FloatRect(500, 275, 560, 340);
}

void Form5DeskMenuState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "editor") {
			App::get().enterState(std::make_shared
				<Form5IntroState>());
		} else if (m_data == "reference") {
			App::get().enterState(std::make_shared
				<Form5ChooseGenderState>());
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

Form5IntroState::Form5IntroState()
	: MenuState("intro") {
	m_buttons["introGirlHover"] = sf::FloatRect(80, 490, 260, 550);
	m_buttons["introBoyHover"] = sf::FloatRect(305, 490, 480, 545);
}

void Form5IntroState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "introBoyHover") {
			App::get().setState(std::make_shared
				<EditorState>("backgroundBoy"));
		} else if (m_data == "introGirlHover") {
			App::get().setState(std::make_shared
				<EditorState>("backgroundGirl"));
		}
		break;
	}
}

void Form5IntroState::update() {
	if (m_checkTexturePartHover(m_buttons["introGirlHover"])) {
		m_data = "introGirlHover";
	} else if (m_checkTexturePartHover(m_buttons["introBoyHover"])) {
		m_data = "introBoyHover";
	} else {
		m_data = "intro";
	}
}

void Form5IntroState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

Form5ChooseGenderState::Form5ChooseGenderState()
	: MenuState("intro") {
	m_buttons["chooseGenderGirlHover"] = sf::FloatRect(80, 490, 260, 550);
	m_buttons["chooseGenderBoyHover"] = sf::FloatRect(305, 490, 480, 545);
}

void Form5ChooseGenderState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			App::get().leaveState();
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (m_data == "chooseGenderBoyHover") {
			App::get().setState(std::make_shared
				<Form5ReferenceState>("referenceBoy"));
		} else if (m_data == "chooseGenderGirlHover") {
			App::get().setState(std::make_shared
				<Form5ReferenceState>("referenceGirl"));
		}
		break;
	}
}

void Form5ChooseGenderState::update() {
	if (m_checkTexturePartHover(m_buttons["chooseGenderGirlHover"])) {
		m_data = "chooseGenderGirlHover";
	} else if (m_checkTexturePartHover(m_buttons["chooseGenderBoyHover"])) {
		m_data = "chooseGenderBoyHover";
	} else {
		m_data = "chooseGender";
	}
}

void Form5ChooseGenderState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_data], true);
	App::get().window().draw(bg);
}

Form5ReferenceState::Form5ReferenceState(const std::string & bg)
	: MenuState("deskMenu")
	, m_button(755, 0, 785, 35)
	, m_bgTextureName(bg) {
}

void Form5ReferenceState::handleEvent(sf::Event event) {
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

void Form5ReferenceState::update() {
	if (m_checkTexturePartHover(m_button)) {
		m_data = "close";
	} else {
		m_data = "";
	}
}

void Form5ReferenceState::render() {
	sf::RectangleShape bg(sf::Vector2f(App::get().window().getSize()));
	bg.setTexture(&TextureHolder::get()[m_bgTextureName], true);
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

