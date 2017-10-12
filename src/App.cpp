#include<Prjoct2/ComponentRenderer.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/ComponentInfo.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/QuadTree.hpp>
#include<Prjoct2/App.hpp>
#include<iostream>
#include<iomanip>

App * App::s_instance = nullptr;

App::App()
	: m_window(sf::VideoMode(
		JSONHolder::get()["settings"]["window"]["width"].get<int>(),
		JSONHolder::get()["settings"]["window"]["height"].get<int>()),
		JSONHolder::get()["settings"]["window"]["title"]
			.get<std::string>(),
		JSONHolder::get()["settings"]["window"]["fullscreen"]
			.get<bool>() ? sf::Style::Fullscreen 
			: sf::Style::Default)
	, m_nextComponentID()
	, m_fieldView(sf::FloatRect(0, 0, m_window.getSize().x,
		m_window.getSize().y)) {
}

App & App::get() {
	if (s_instance == nullptr) {
		s_instance = new App();
	}
	return *s_instance;
}

void App::run() {
	while (m_window.isOpen()) {
		m_handleEvents();
		m_render();
		m_update();
	}
}

void App::m_handleEvents() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (GUIHolder::get().gui().handleEvent(event) && event.type
			!= sf::Event::KeyPressed) {
			continue;
		}
		switch (event.type) {
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::KeyPressed:
			m_handleKeyEvent(event.key);
			break;
		case sf::Event::MouseButtonPressed:
			m_handleMousePressEvent(event.mouseButton);
			break;
		case sf::Event::MouseButtonReleased:
			m_handleMouseReleaseEvent(event.mouseButton);
			break;
		}
	}
}

void App::m_handleMouseReleaseEvent(const sf::Event::MouseButtonEvent & event) {
	switch (event.button) {
	case sf::Mouse::Left:
		break;
	case sf::Mouse::Right:
		break;
	}
}

void App::m_selectComponent(int x, int y) {
	sf::Vector2f mouse(mapToFieldCoords(sf::Vector2i(x, y)));
	std::string id = QuadTree::get().getComponentID(mouse);
	if (id == "") {
		return;
	}
	JSONHolder::get()["selected component ID"] = id;
	GUIHolder::get().createContextMenu(x, y);
}

void App::m_placeNewComponent(int pressX, int pressY) {
	auto idString = std::to_string(m_nextComponentID);
	auto & component = JSONHolder::get()["components"][idString];
	component = JSONHolder::get()["current"];
	component["id"] = m_nextComponentID;
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Vector2i(pressX, pressY))
		/ float(cellsize));
	component["position"]["x"] = mouse.x;
	component["position"]["y"] = mouse.y;
	std::string currentType = component["type"];
	if (currentType != "dot") {
		for (nlohmann::json & pin : component["pins"]) {
			pin["parentID"] = idString;
		}
	}
	auto rect = ComponentInfo::getRect(idString);
	if (QuadTree::get().intersects(rect) == "") {
		QuadTree::get().addObject(idString);
		++m_nextComponentID;
		JSONHolder::get()["current"] = nlohmann::json();
	} else {
		JSONHolder::get()["components"].erase(idString);
	}
}

void App::m_placeMovingComponent(int pressX, int pressY) {
	std::string id = JSONHolder::get()["moving"]["id"];
	nlohmann::json & component = JSONHolder::get()["components"][id];
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Vector2i(pressX, pressY))
		/ float(cellsize));
	component["position"]["x"] = mouse.x;
	component["position"]["y"] = mouse.y;
	auto rect = ComponentInfo::getRect(component);
	if (QuadTree::get().intersects(rect) == "") {
		component.erase("moving");
		JSONHolder::get()["moving"] = nlohmann::json();
		QuadTree::get().addObject(id);
	}
}

void App::m_handleMousePressEvent(const sf::Event::MouseButtonEvent & event) {
	GUIHolder::get().removeContextMenu();
	switch (event.button) {
	case sf::Mouse::Left:
		if (!JSONHolder::get()["moving"].is_null()) {
			m_placeMovingComponent(event.x, event.y);
		} else if (JSONHolder::get()["current"].is_null()) {
			m_selectPin(event.x, event.y);
		} else {
			m_deselectPin();
			m_placeNewComponent(event.x, event.y);
		}
		break;
	case sf::Mouse::Right:
		if (JSONHolder::get()["moving"].is_null()) {
			m_selectComponent(event.x, event.y);
		}
		break;
	}
}

void App::m_handleKeyEvent(const sf::Event::KeyEvent & event) {
	GUIHolder::get().removeContextMenu();
	switch (event.code) {
	case sf::Keyboard::Escape:
		if (!m_cancelStartedAction()) {
			m_window.close();
		}
		break;
	case sf::Keyboard::Space:
		JSONHolder::get()["current"]
			= JSONHolder::get()["components/lamp"];
		break;
	case sf::Keyboard::D:
		std::cout << "++++++++++++++++++++++++++++++++++++++++++\n"
			<< std::setw(4) 
			<< JSONHolder::get()["components"] << std::endl
			<< "###########################################\n";
		break;
	case sf::Keyboard::P:
		std::cout << std::setw(4)
			<< JSONHolder::get()["selected pin"] << std::endl;
		break;
	case sf::Keyboard::Q:
		QuadTree::get().dump();
		break;
	case sf::Keyboard::M:
		std::cout << std::setw(4)
			<< JSONHolder::get()["moving"] << std::endl;
		break;
	case sf::Keyboard::Delete:
		if (!JSONHolder::get()["selected pin"].is_null()) {
			m_disconnectSelectedPin();
			m_deselectPin();
		}
		break;
	case sf::Keyboard::Z:
		m_handleZKeyPress(event);
		break;
	}
}

void App::m_handleZKeyPress(const sf::Event::KeyEvent & zKey) {
	if (!zKey.control) {
		return;
	}
	if (zKey.shift) {
		m_redo();
	} else {
		m_undo();
	}
}

void App::m_undo() {
	if (m_cancelStartedAction()) {
		return;
	}
}

void App::m_redo() {
}

bool App::m_cancelStartedAction() {
	if (!JSONHolder::get()["current"].is_null()) {
		JSONHolder::get()["current"] = nlohmann::json();
		return true;
	} else if (!JSONHolder::get()["moving"].is_null()) {
		cancelMovingComponent();
		return true;
	} else if (!JSONHolder::get()["selected pin"].is_null()) {
		m_deselectPin();
		return true;
	}
	return false;
}

void App::m_render() {
	m_window.clear(sf::Color::White);
	setFieldView();
	m_renderComponents();
	ComponentRenderer::get().drawCurrent();
	resetView();
	GUIHolder::get().gui().draw();
	m_window.display();
}

void App::m_update() {
	m_updateFieldView();
	m_updateMovingComponent();
}

void App::m_updateMovingComponent() {
	if (JSONHolder::get()["moving"].is_null()) {
		return;
	}
	print(JSONHolder::get()["moving"]);
	std::string id = JSONHolder::get()["moving"]["id"];
	nlohmann::json & component = JSONHolder::get()["components"][id];
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Mouse::getPosition(m_window))
		/ float(cellsize));
	component["position"]["x"] = mouse.x;
	component["position"]["y"] = mouse.y;
}

void App::m_updateFieldView() {
	sf::Vector2f offset;
	float speed = 0.5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		offset.y -= speed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		offset.y += speed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		offset.x -= speed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		offset.x += speed;
	}
	m_fieldView.move(offset);
}

void App::m_renderComponents() {
	for (auto component : JSONHolder::get()["components"]) {
		ComponentRenderer::get().drawComponent(component);
	}
}

sf::RenderWindow & App::window() {
	return m_window;
}

void App::setFieldView() {
	m_window.setView(m_fieldView);
}

void App::resetView() {
	m_window.setView(m_window.getDefaultView());
}

void App::m_selectPin(int pressX, int pressY) {
	size_t cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(m_window.mapPixelToCoords(sf::Vector2i(pressX,
		pressY), m_fieldView) / float(cellsize) 
		+ sf::Vector2f(.5f, .5f));
	auto pinPtr = QuadTree::get().getPin(mouse.x, mouse.y);
	nlohmann::json & selectedPin = JSONHolder::get()["selected pin"];
	if (pinPtr == nullptr) {
		m_deselectPin();
	} else if (selectedPin.is_null()) {
		m_deselectPin();
		selectedPin = *pinPtr;
	} else {
		if ((*pinPtr)["parentID"] == selectedPin["parentID"]) {
			m_deselectPin();
			selectedPin = *pinPtr;
		} else {
			nlohmann::json * selectedPinPtr;
			if (selectedPin.count("temporary") == 1) {
				std::string id = selectedPin["id"];
				std::string parentID = selectedPin["parentID"];
				selectedPinPtr = &JSONHolder::get()
					["components"][parentID]["pins"][id];
			} else {
				selectedPinPtr = ComponentInfo::getComponentPin(
					selectedPin["parentID"],
					selectedPin["x"], selectedPin["y"]);
				m_disconnectPin(*selectedPinPtr);
			}
			m_disconnectPin(*pinPtr);
			m_connectPins(*pinPtr, *selectedPinPtr);	
			m_connectPins(*selectedPinPtr, *pinPtr);
			selectedPin = nlohmann::json();
		}
	}
}

void App::m_deselectPin() {
	nlohmann::json & selected = JSONHolder::get()["selected pin"];
	if (!selected.is_null() && selected.count("temporary") == 1) {
		std::string parentID = selected["parentID"];
		nlohmann::json & parent = JSONHolder::get()["components"]
			[parentID];
		std::string id = selected["id"];
		parent["pins"].erase(id);
	}
	selected = nlohmann::json();
}

void App::m_disconnectPin(nlohmann::json & pin) {
	if (pin["connection"].is_null()) {
		return;
	}
	if (pin["connection"].count("id") == 1) {
		std::string otherParentID = pin["connection"]["parentID"];
		std::string otherID = pin["connection"]["id"];
		JSONHolder::get()["components"][otherParentID]["pins"]
			.erase(otherID);
	} else {
		nlohmann::json & other = *(ComponentInfo::getComponentPin(
			pin["connection"]["parentID"], pin["connection"]["x"],
			pin["connection"]["y"]));
		other.erase("connection");
	}
	if (pin.count("temporary") == 1) {
		std::string parentID = pin["parentID"];
		std::string id = pin["id"];
		JSONHolder::get()["components"][parentID]["pins"]
			.erase(id);
	} else {
		pin.erase("connection");
	}
}

void App::m_disconnectSelectedPin() {
	nlohmann::json & selectedPin = JSONHolder::get()["selected pin"];
	if (selectedPin.count("temporary") == 1) {
		nlohmann::json & parent = JSONHolder::get()["components"]
			[selectedPin["parentID"].get<std::string>()];
		for (nlohmann::json & pin : parent["pins"]) {
			m_disconnectPin(pin);
		}
	} else {
		nlohmann::json * pinPtr = ComponentInfo::getComponentPin(
			selectedPin["parentID"].get<std::string>(),
			selectedPin["x"].get<int>(),
			selectedPin["y"].get<int>());
		m_disconnectPin(*pinPtr);
	}
}

void App::m_connectPins(nlohmann::json & pin1, nlohmann::json & pin2) {
	nlohmann::json & connection = pin1["connection"];
	connection["parentID"] = pin2["parentID"];
	connection["x"] = pin2["x"];
	connection["y"] = pin2["y"];
	if (pin2.count("temporary") == 1) {
		connection["id"] = pin2["id"];
	}
}

sf::Vector2f App::mapToFieldCoords(sf::Vector2i pixel) {
	return m_window.mapPixelToCoords(pixel, m_fieldView);
}

void App::deleteSelectedComponent() {
	std::string id = JSONHolder::get()["selected component ID"];
	if (id == "") {
		return;
	}
	nlohmann::json & component = JSONHolder::get()["components"][id];
	for (nlohmann::json & pin : component["pins"]) {
		m_disconnectPin(pin);
	}
	QuadTree::get().removeObject(id);
	JSONHolder::get()["components"].erase(id);
	JSONHolder::get()["selected component ID"] = nlohmann::json();
}

void App::moveSelectedComponent() {
	std::string id = JSONHolder::get()["selected component ID"];
	if (id == "") {
		return;
	}
	if (!JSONHolder::get()["selected pin"].is_null()) {
		m_deselectPin();
	}
	nlohmann::json & component = JSONHolder::get()["components"][id];
	component["moving"] = true;
	JSONHolder::get()["moving"]["id"] = id;
	JSONHolder::get()["moving"]["position"] = component["position"];
	QuadTree::get().removeObject(id);
}

void App::cancelMovingComponent() {
	if (JSONHolder::get()["moving"].is_null()) {
		return;
	}
	std::string id = JSONHolder::get()["moving"]["id"];
	if (id == "") {
		return;
	}
	nlohmann::json & component = JSONHolder::get()["components"][id];
	component["position"] = JSONHolder::get()["moving"]["position"];
	component.erase("moving");
	JSONHolder::get()["moving"] = nlohmann::json();
	QuadTree::get().addObject(id);
}

