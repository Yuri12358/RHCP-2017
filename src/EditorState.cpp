#include<Prjoct2/ComponentRenderer.hpp>
#include<Prjoct2/ComponentInfo.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/EditorState.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/MainMenuState.hpp>
#include<Prjoct2/QuadTree.hpp>
#include<Prjoct2/History.hpp>
#include<Prjoct2/App.hpp>
#include<fstream>

EditorState::EditorState()
	: m_locked()
	, m_fieldView(sf::FloatRect(0, 0, App::get().m_window.getSize().x,
		App::get().m_window.getSize().y))
	, m_nextComponentID() {
	GUIHolder::get().initEditorGUI();
	createNewCircuit();
}

void EditorState::handleEvent(sf::Event event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		m_handleKeyEvent(event.key);
		break;
	case sf::Event::MouseButtonPressed:
		m_handleMousePressEvent(event.mouseButton);
		break;
	}
}

void EditorState::m_handleKeyEvent(sf::Event::KeyEvent event) {
	if (!JSONHolder::get()["editing component ID"].is_null()) {
		return;
	}
	GUIHolder::get().removeContextMenu();
	switch (event.code) {
	case sf::Keyboard::Escape:
		if (!m_cancelStartedAction()) {
			App::get().m_state = std::make_shared<MainMenuState>();
		}
		break;
	case sf::Keyboard::Space:
		JSONHolder::get()["current"] = JSONHolder::get()["components/"
			+ JSONHolder::get()["next component type"]
			.get<std::string>()];
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

void EditorState::m_handleZKeyPress(const sf::Event::KeyEvent & zKey) {
	if (!zKey.control) {
		return;
	}
	if (zKey.shift) {
		m_cancelStartedAction();
		History::get().redo();
	} else {
		if (m_cancelStartedAction()) {
			return;
		}
		History::get().undo();
	}
}

void EditorState::m_handleMousePressEvent(sf::Event::MouseButtonEvent event) {
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

void EditorState::render() {
	m_renderBackground();
	App::get().m_window.setView(m_fieldView);
	m_renderComponents();
	ComponentRenderer::get().drawCurrent();
	App::get().m_window.setView(App::get().m_defaultView);
}

void EditorState::m_renderBackground() {
	sf::RectangleShape shape(sf::Vector2f(App::get().m_window.getSize()));
	shape.setTexture(&TextureHolder::get()["background"], true);
	shape.setFillColor(sf::Color(255, 255, 255, 192));
	App::get().m_window.draw(shape);
}

void EditorState::m_renderComponents() {
	for (auto component : JSONHolder::get()["components"]) {
		ComponentRenderer::get().drawComponent(component);
	}
}

void EditorState::update() {
	if (m_locked) {
		return;
	}
	m_updateScrolling();
	m_updateMovingComponent();
}

void EditorState::m_updateScrolling() {
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

void EditorState::m_updateMovingComponent() {
	if (JSONHolder::get()["moving"].is_null()) {
		return;
	}
	std::string id = JSONHolder::get()["moving"]["id"];
	nlohmann::json & component = JSONHolder::get()["components"][id];
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Mouse::getPosition(App::get()
		.m_window)) / float(cellsize));
	component["position"]["x"] = mouse.x;
	component["position"]["y"] = mouse.y;
}

void EditorState::m_placeNewComponent(int pressX, int pressY) {
	if (m_locked) {
		return;
	}
	History::get().beginModification();
	auto idString = std::to_string(m_nextComponentID);
	auto & component = JSONHolder::get()["components"][idString];
	component = JSONHolder::get()["current"];
	component["id"] = idString;
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Vector2i(pressX, pressY))
		/ float(cellsize));
	component["position"]["x"] = mouse.x;
	component["position"]["y"] = mouse.y;
	component["rotation"] = 0;
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
		History::get().endModification();
	} else {
		JSONHolder::get()["components"].erase(idString);
		History::get().abortModification();
	}
}

void EditorState::m_placeMovingComponent(int pressX, int pressY) {
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
		nlohmann::json beforeMoving = component;
		beforeMoving["position"] = JSONHolder::get()["moving"]
			["position"];
		History::get().endModification();
		JSONHolder::get()["moving"] = nlohmann::json();
		QuadTree::get().addObject(id);
	}
}

void EditorState::m_selectPin(int pressX, int pressY) {
	size_t cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::Vector2i mouse(mapToFieldCoords(sf::Vector2i(pressX, pressY))
		/ float(cellsize) + sf::Vector2f(.5f, .5f));
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
			History::get().beginModification();
			nlohmann::json * selectedPinPtr;
			if (selectedPin.count("temporary") == 1) {
				std::string id = selectedPin["id"];
				std::string parentID = selectedPin["parentID"];
				selectedPinPtr = &JSONHolder::get()
					["components"][parentID]["pins"][id];
			} else {
				selectedPinPtr = ComponentInfo
					::getComponentPin(selectedPin
					["parentID"], selectedPin["x"],
					selectedPin["y"]);
				m_disconnectPin(*selectedPinPtr);
			}
			m_disconnectPin(*pinPtr);
			m_connectPins(*pinPtr, *selectedPinPtr);	
			m_connectPins(*selectedPinPtr, *pinPtr);
			selectedPin = nlohmann::json();
			History::get().endModification();
		}
	}
}

void EditorState::cancelMovingComponent() {
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
	History::get().abortModification();
}

void EditorState::deleteSelectedComponent() {
	std::string id = JSONHolder::get()["selected component ID"];
	deleteComponent(id);
	JSONHolder::get()["selected component ID"] = nlohmann::json();
}

void EditorState::deleteComponent(const std::string & id) {
	if (JSONHolder::get()["components"].count(id) == 0) {
		return;
	}
	nlohmann::json & component = JSONHolder::get()["components"][id];
	History::get().beginModification();
	for (nlohmann::json & pin : component["pins"]) {
		m_disconnectPin(pin);
	}
	QuadTree::get().removeObject(id);
	JSONHolder::get()["components"].erase(id);
	History::get().endModification();
	if (!JSONHolder::get()["editing component ID"].is_null()
		&& JSONHolder::get()["editing component ID"].get<std::string>()
		== id) {
		GUIHolder::get().closeComponentPropertyEditor();
	}
}

void EditorState::rotateSelectedComponent() {
	History::get().beginModification();
	std::string id = JSONHolder::get()["selected component ID"];
	if (id == "") {
		return;
	}
	if (!JSONHolder::get()["selected pin"].is_null()) {
		m_deselectPin();
	}
	nlohmann::json & component = JSONHolder::get()["components"][id];
	QuadTree::get().removeObject(id);
	component["rotation"] = (1 + (component["rotation"].get<int>())) % 4;
	QuadTree::get().addObject(id);
	History::get().endModification();
}

void EditorState::moveSelectedComponent() {
	History::get().beginModification();
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

void EditorState::createNewCircuit() {
	if (m_locked) {
		return;
	}
	App::get().m_currentCircuitName = "";
	GUIHolder::get().updateFileName();
	History::get().clear();
	QuadTree::get().removeAll();
	JSONHolder::get()["components"].clear();
	m_cancelStartedAction();
}

void EditorState::openCircuit() {
	if (m_locked) {
		return;
	}
	GUIHolder::get().createOpenFileDialogWindow();
	m_locked = true;
}

void EditorState::openCircuit(const std::string & name) {
	std::ifstream file("saves/" + name + ".sav", std::ifstream::binary);
	if (!file.is_open()) {
		return;
	}
	file.seekg(0, file.end);
	int size = file.tellg();
	file.seekg(0);
	std::vector<uint8_t> cbor(size);
	for (auto & byte : cbor) {
		byte = file.get();
	}
	int maxNextID = m_nextComponentID;
	JSONHolder::get()["components"] = nlohmann::json::from_cbor(cbor);
	for (const auto & component : JSONHolder::get()["components"]) {
		int id = std::stoi(component["id"].get<std::string>());
		maxNextID = std::max(id + 1, maxNextID);
	}
	m_nextComponentID = maxNextID;
	QuadTree::get().addAll();
	App::get().m_currentCircuitName = name;
	GUIHolder::get().updateFileName(name);
	unlockUI();
	GUIHolder::get().closeDialogWindow();
	m_cancelStartedAction();
}

void EditorState::saveCircuit() {
	if (m_locked) {
		return;
	}
	if (App::get().m_currentCircuitName != "") {
		saveCircuit(App::get().m_currentCircuitName);
		return;
	}
	GUIHolder::get().createSaveFileDialogWindow();
	m_locked = false;
}

void EditorState::saveCircuit(const std::string & name) {
	std::ofstream file("saves/" + name + ".sav", std::ofstream::binary);
	const auto & cbor = nlohmann::json::to_cbor(
		JSONHolder::get()["components"]);
	for (auto byte : cbor) {
		file.put(byte);
	}
	App::get().m_currentCircuitName = name;
	GUIHolder::get().updateFileName(name);
	unlockUI();
	GUIHolder::get().closeDialogWindow();
	m_cancelStartedAction();
}

bool EditorState::isUILocked() {
	return m_locked;
}

void EditorState::unlockUI() {
	m_locked = false;
}

void EditorState::updateFieldView() {
	sf::Vector2f offset = (App::get().m_defaultView.getSize() - m_fieldView
		.getSize()) / 2.f;
	m_fieldView.setSize(App::get().m_defaultView.getSize());
	m_fieldView.move(offset);
}

sf::Vector2f EditorState::mapToFieldCoords(sf::Vector2i pixel) {
	return App::get().m_window.mapPixelToCoords(pixel, m_fieldView);
}

bool EditorState::m_cancelStartedAction() {
	bool canceled = false;
	if (!JSONHolder::get()["current"].is_null()) {
		JSONHolder::get()["current"] = nlohmann::json();
		canceled = true;
	}
	if (!JSONHolder::get()["moving"].is_null()) {
		cancelMovingComponent();
		canceled = true;
	}
	if (!JSONHolder::get()["selected pin"].is_null()) {
		m_deselectPin();
		canceled = true;
	}
	if (!JSONHolder::get()["editing component ID"].is_null()) {
		GUIHolder::get().closeComponentPropertyEditor();
		canceled = true;
	}
	if (!JSONHolder::get()["selected component ID"].is_null()) {
		GUIHolder::get().removeContextMenu();
		canceled = true;
	}
	return canceled;
}

void EditorState::m_selectComponent(int x, int y) {
	if (m_locked) {
		return;
	}
	sf::Vector2f mouse(mapToFieldCoords(sf::Vector2i(x, y)));
	std::string id = QuadTree::get().getComponentID(mouse);
	if (id == "") {
		return;
	}
	JSONHolder::get()["selected component ID"] = id;
	GUIHolder::get().createContextMenu(x, y);
}

void EditorState::m_deselectPin() {
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

void EditorState::m_disconnectPin(nlohmann::json & pin) {
	if (pin["connection"].is_null()) {
		return;
	}
	nlohmann::json otherCopy;
	if (pin["connection"].count("id") == 1) {
		std::string otherParentID = pin["connection"]["parentID"];
		std::string otherID = pin["connection"]["id"];
		otherCopy = JSONHolder::get()["components"][otherParentID]
			["pins"][otherID];
		otherCopy.erase("connection");
		JSONHolder::get()["components"][otherParentID]["pins"]
			.erase(otherID);
	} else {
		nlohmann::json & other = *(ComponentInfo::getComponentPin(
			pin["connection"]["parentID"], pin["connection"]["x"],
			pin["connection"]["y"]));
		other.erase("connection");
		otherCopy = other;
	}
	nlohmann::json pinCopy = pin;
	pinCopy.erase("connection");
	if (pin.count("temporary") == 1) {
		std::string parentID = pin["parentID"];
		std::string id = pin["id"];
		JSONHolder::get()["components"][parentID]["pins"]
			.erase(id);
	} else {
		pin.erase("connection");
	}
}

void EditorState::m_disconnectSelectedPin() {
	History::get().beginModification();
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
	History::get().endModification();
}

void EditorState::m_connectPins(nlohmann::json & pin1, nlohmann::json & pin2) {
	nlohmann::json & connection = pin1["connection"];
	connection["parentID"] = pin2["parentID"];
	connection["x"] = pin2["x"];
	connection["y"] = pin2["y"];
	if (pin2.count("temporary") == 1) {
		connection["id"] = pin2["id"];
	}
}

