#include<Prjoct2/ComponentRenderer.hpp>
#include<Prjoct2/ComponentInfo.hpp>
#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/QuadTree.hpp>
#include<Prjoct2/App.hpp>
#include<SFML/Graphics.hpp>

ComponentRenderer * ComponentRenderer::s_instance = nullptr;

ComponentRenderer::ComponentRenderer() {
}

ComponentRenderer & ComponentRenderer::get() {
	if (s_instance == nullptr) {
		s_instance = new ComponentRenderer();
	}
	return *s_instance;
}

void ComponentRenderer::drawCurrent() {
	if (JSONHolder::get()["current"].is_null()) {
		return;
	}
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	const auto & current = JSONHolder::get()["current"];
	sf::RectangleShape shape(sf::Vector2f(
		static_cast<int>(current["width"]) * cellsize,
		static_cast<int>(current["height"]) * cellsize));
	shape.setOutlineThickness(-1);
	shape.setTexture(&(TextureHolder::get()[current["texture"]]), true);
	sf::Vector2i mouse(App::get().mapToFieldCoords(
		sf::Mouse::getPosition(App::get().window())));
	sf::Vector2i position(mouse / cellsize);
	sf::IntRect rect(position.x, position.y,
			static_cast<int>(current["width"]),
			static_cast<int>(current["height"]));
	if (QuadTree::get().intersects(rect) != "") {
		shape.setFillColor(sf::Color(255, 0, 0, 64));
		shape.setOutlineColor(sf::Color::Red);
	} else {
		shape.setFillColor(sf::Color(255, 255, 0, 64));
		shape.setOutlineColor(sf::Color::Yellow);
	}
	shape.setPosition(sf::Vector2f(position * int(cellsize)));
	App::get().window().draw(shape);
}

void ComponentRenderer::m_drawWire(nlohmann::json & pin) {
	int cellsize = JSONHolder::get()["settings"]["cellsize"];

	sf::VertexArray wire(sf::LineStrip, 4);
	std::string parentID = pin["parentID"];

	nlohmann::json & parent = JSONHolder::get()["field"]["contents"]
		[parentID];

	sf::Vector2f pos = sf::Vector2f(static_cast<int>(parent["position"]
		["x"]) + static_cast<int>(pin["x"]), static_cast<int>(parent
		["position"]["y"]) + static_cast<int>(pin["y"]));

	wire[0].position = pos * float(cellsize);

	nlohmann::json & conn = pin["connection"];
	std::string otherParentID = conn["parentID"];
	
	nlohmann::json & other = JSONHolder::get()["field"]["contents"]
		[otherParentID];
	
	nlohmann::json & otherPin = *(ComponentInfo::getComponentPin(
		conn["parentID"], conn["x"], conn["y"]));
	sf::Vector2f otherPos = sf::Vector2f(static_cast<int>(other["position"]
		["x"]) + static_cast<int>(otherPin["x"]), static_cast<int>(other
		["position"]["y"]) + static_cast<int>(otherPin["y"]));
	
	// There will be a lot of "if's"
	
	Direction dir1 = ComponentInfo::getPinOrientation(pin), dir2 = ComponentInfo::getPinOrientation(otherPin);

	if((dir1 & (Left | Right)) && (dir1 & getOppositeDirection(dir2))) {
		sf::Vector2f left = otherPos, right = pos;
		if ((dir1 & Left) && !(dir2 & Left)) {
			left = pos;
			right = otherPos;
		}
		if ((left.x - right.x) > 0) {
			wire[1].position = sf::Vector2f((pos.x + otherPos.x) / 2, pos.y) * float(cellsize);
			wire[2].position = sf::Vector2f((pos.x + otherPos.x) / 2, otherPos.y) * float(cellsize);
		} else {
			wire[1].position = sf::Vector2f(pos.x, (pos.y + otherPos.y) / 2) * float(cellsize);
			wire[2].position = sf::Vector2f(otherPos.x, (pos.y + otherPos.y) / 2) * float(cellsize);
		}
	}
	else if((dir1 & (Up | Down)) && (dir1 & getOppositeDirection(dir2))) {
		sf::Vector2f up = otherPos, down = pos;
		if((dir1 & Up) && !(dir2 & Up)) {
			up = pos;
			down = otherPos;
		}
		if((up.y - down.y) > 0) {
			wire[1].position = sf::Vector2f(pos.x, (pos.y + otherPos.y) / 2) * float(cellsize);
			wire[2].position = sf::Vector2f(otherPos.x, (pos.y + otherPos.y) / 2) * float(cellsize);
		} else {
			wire[1].position = sf::Vector2f((pos.x + otherPos.x) / 2, pos.y) * float(cellsize);
			wire[2].position = sf::Vector2f((pos.x + otherPos.x) / 2, otherPos.y) * float(cellsize);
		}
	}
	else if((dir1 & (Left | Right)) && (dir1 & dir2))
	{
		sf::Vector2f left = otherPos, right = pos;
		if((dir1 & Right))
		{
			left = pos;
			right = otherPos;
		}
		if((right.x - left.x) > 0)
		{
			wire[1].position = wire[2].position = sf::Vector2f(otherPos.x, pos.y) * float(cellsize);
		}
		else
		{
			wire[1].position = wire[2].position = sf::Vector2f(pos.x, otherPos.y) * float(cellsize);
		}
	}
	else if((dir1 & (Up | Down)) && (dir1 & dir2))
	{
		sf::Vector2f up = otherPos, down = pos;
		if((dir1 & Down))
		{
			up = pos;
			down = otherPos;
		}
		if((up.x - down.x) > 0)
		{
			wire[1].position = wire[2].position = sf::Vector2f(pos.x, otherPos.y) * float(cellsize);
		}
		else
		{
			wire[1].position = wire[2].position = sf::Vector2f(otherPos.x, pos.y) * float(cellsize);
		}
	}

	wire[3].position = otherPos * float(cellsize);
	
	wire[0].color = wire[1].color = wire[2].color = wire[3].color = sf::Color::Black;
	otherPin["connection"]["rendered"] = true;
	App::get().window().draw(wire);
}

void ComponentRenderer::m_drawPins(nlohmann::json & component) {
	sf::CircleShape pinShape(4);
	pinShape.setFillColor(sf::Color::Black);
	pinShape.setOrigin(4, 4);
	sf::Vector2i position(static_cast<int>(component["position"]["x"]),
		static_cast<int>(component["position"]["y"]));
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	nlohmann::json & selectedPin(JSONHolder::get()["selected pin"]);
	for (auto pin : component["pins"]) {
		if (pin["connection"].is_null()) {
			if (!selectedPin.is_null()
				&& pin["parentID"] == selectedPin["parentID"]
				&& pin["x"] == selectedPin["x"]
				&& pin["y"] == selectedPin["y"]) {
				pinShape.setFillColor(sf::Color::Green);
			} else {
				pinShape.setFillColor(sf::Color::Black);
			}
			sf::Vector2i relPos(static_cast<int>(pin["x"]),
				static_cast<int>(pin["y"]));
			pinShape.setPosition(sf::Vector2f((position + relPos) *
				cellsize));
			App::get().window().draw(pinShape);
		} else if (pin["connection"]["rendered"].is_null()) {
			m_drawWire(pin);
		} else {
			pin["connection"]["rendered"] = nlohmann::json();
		}
	}
}

void ComponentRenderer::drawComponent(nlohmann::json & component) {
	int cellsize = JSONHolder::get()["settings"]["cellsize"];
	sf::RectangleShape shape(sf::Vector2f(
		static_cast<int>(component["width"]) * cellsize,
		static_cast<int>(component["height"]) * cellsize));
	shape.setTexture(
		&(TextureHolder::get()[component["texture"]]), true);
	sf::Vector2i position(
		static_cast<int>(component["position"]["x"]),
		static_cast<int>(component["position"]["y"]));
	shape.setPosition(sf::Vector2f(position * cellsize));
	App::get().window().draw(shape);
	m_drawPins(component);
}

