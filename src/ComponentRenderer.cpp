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

	nlohmann::json & parent = JSONHolder::get()["components"]
		[parentID];

	auto relPinPos = ComponentInfo::rotatePin(pin);

	sf::Vector2f pos = sf::Vector2f(static_cast<int>(parent["position"]
		["x"]) + relPinPos.x, static_cast<int>(parent
		["position"]["y"]) + relPinPos.y);

	wire[0].position = pos * float(cellsize);

	nlohmann::json & conn = pin["connection"];
	std::string otherParentID = conn["parentID"];
	
	nlohmann::json & other = JSONHolder::get()["components"]
		[otherParentID];
	
	nlohmann::json * otherPinPtr = nullptr;
	if (conn.count("id") == 1) {
		otherPinPtr = &JSONHolder::get()["components"]
			[conn["parentID"].get<std::string>()]["pins"]
			[conn["id"].get<std::string>()];
	} else {
		otherPinPtr = ComponentInfo::getComponentPin(
			conn["parentID"], conn["x"], conn["y"]);
	}
	nlohmann::json & otherPin = *otherPinPtr;

	auto otherRelPinPos = ComponentInfo::rotatePin(otherPin);

	sf::Vector2f otherPos = sf::Vector2f(static_cast<int>(other["position"]
		["x"]) + otherRelPinPos.x, static_cast<int>(other
		["position"]["y"]) + otherRelPinPos.y);

	wire[3].position = otherPos * float(cellsize);
	
	// There will be a lot of "if's"
	
	Direction dir1 = ComponentInfo::getPinOrientation(pin);
	Direction dir2 = ComponentInfo::getPinOrientation(otherPin);
	
	auto pos1 = pos, pos2 = otherPos;

	int index1 = 1, index2 = 2;

	if (dir1 == All) {
		std::swap(dir1, dir2);
		std::swap(pos1, pos2);
		std::swap(index1, index2);
	}
	
	if (dir2 == All) {
		if (dir1 & Left) {
			if (pos1.x < pos2.x) {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos1.x, pos2.y)
					* float(cellsize);
			} else {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos2.x, pos1.y)
					* float(cellsize);
			}
		} else if (dir1 & Up) {
			if (pos1.y > pos2.y) {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos1.x, pos2.y)
					* float(cellsize);
			} else {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos2.x, pos1.y)
					* float(cellsize);
			}	
		} else if (dir1 & Right) {
			if (pos1.x < pos2.x) {
				wire[index1].position = wire[index2].position 
					= sf::Vector2f(pos2.x, pos1.y)
					* float(cellsize);
			} else {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos1.x, pos2.y)
					* float(cellsize);
			}
		} else if (dir1 & Down) {
			if (pos1.y > pos2.y) {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos2.x, pos1.y)
					* float(cellsize);
			} else {
				wire[index1].position = wire[index2].position
					= sf::Vector2f(pos1.x, pos2.y)
					* float(cellsize);
			}
		}
	} else if ((dir1 & (Left | Right))
		&& (dir1 & getOppositeDirection(dir2))) {
		sf::Vector2f left = otherPos, right = pos;
		if ((dir1 & Left) && !(dir2 & Left)) {
			left = pos;
			right = otherPos;
		}
		if ((left.x - right.x) > 0) {
			wire[1].position = sf::Vector2f((pos.x + otherPos.x)
				/ 2, pos.y) * float(cellsize);
			wire[2].position = sf::Vector2f((pos.x + otherPos.x)
				/ 2, otherPos.y) * float(cellsize);
		} else {
			wire[1].position = sf::Vector2f(pos.x, (pos.y
				+ otherPos.y) / 2) * float(cellsize);
			wire[2].position = sf::Vector2f(otherPos.x, (pos.y
				+ otherPos.y) / 2) * float(cellsize);
		}
	} else if ((dir1 & (Up | Down)) && (dir1 & getOppositeDirection(
		dir2))) {
		sf::Vector2f up = otherPos, down = pos;
		if ((dir1 & Up) && !(dir2 & Up)) {
			up = pos;
			down = otherPos;
		}
		if ((up.y - down.y) > 0) {
			wire[1].position = sf::Vector2f(pos.x, (pos.y 
				+ otherPos.y) / 2) * float(cellsize);
			wire[2].position = sf::Vector2f(otherPos.x, (pos.y
				+ otherPos.y) / 2) * float(cellsize);
		} else {
			wire[1].position = sf::Vector2f((pos.x + otherPos.x)
				/ 2, pos.y) * float(cellsize);
			wire[2].position = sf::Vector2f((pos.x + otherPos.x)
				/ 2, otherPos.y) * float(cellsize);
		}
	} else if ((dir1 & (Left | Right)) && (dir1 & dir2)) {
		sf::Vector2f left = otherPos, right = pos;
		if ((dir1 & Right)) {
			left = pos;
			right = otherPos;
		}
		if ((right.x - left.x) > 0) {
			wire[1].position = wire[2].position = sf::Vector2f(
				otherPos.x, pos.y) * float(cellsize);
		} else {
			wire[1].position = wire[2].position = sf::Vector2f(
				pos.x, otherPos.y) * float(cellsize);
		}
	} else if ((dir1 & (Up | Down)) && (dir1 & dir2)) {
		sf::Vector2f up = otherPos, down = pos;
		if ((dir1 & Down)) {
			up = pos;
			down = otherPos;
		}
		if ((up.y - down.y) < 0) {
			wire[1].position = wire[2].position = sf::Vector2f(
				pos.x, otherPos.y) * float(cellsize);
		} else {
			wire[1].position = wire[2].position = sf::Vector2f(
				otherPos.x, pos.y) * float(cellsize);
		}
	} else {
		auto Dir1 = dir1, Dir2 = dir2;
		auto Pos = pos, OtherPos = otherPos;
		int vertex1 = 1, vertex2 = 2;
		if ((dir2 & rotateDirection(dir1, -1))) {
			Dir1 = dir2;
			Dir2 = dir1;
			Pos = otherPos;
			OtherPos = pos;
			vertex1 = 2;
			vertex2 = 1;
		}
		if (Dir1 == Left) {
			if (Pos.x > OtherPos.x) {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(OtherPos.x, Pos.y) * float(cellsize);
				} else {
					wire[vertex1].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, Pos.y) * float(cellsize);
					wire[vertex2].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, OtherPos.y) * float(cellsize);
				}
			} else {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = sf::Vector2f(Pos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
					wire[vertex2].position = sf::Vector2f(OtherPos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
				} else {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(Pos.x, OtherPos.y) * float(cellsize);
				}
			}
		} else if (Dir1 == Right) {
			if (Pos.x > OtherPos.x) {
				if (Pos.y < OtherPos.y) { 
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(Pos.x, OtherPos.y) * float(cellsize);
				} else {
					wire[vertex1].position = sf::Vector2f(Pos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
					wire[vertex2].position = sf::Vector2f(OtherPos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
				}
			} else {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, Pos.y) * float(cellsize);
					wire[vertex2].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, OtherPos.y) * float(cellsize);
				} else {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(OtherPos.x, Pos.y) * float(cellsize);
				}
			}
		} else if (Dir1 == Up) {
			if (Pos.x > OtherPos.x) {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, Pos.y) * float(cellsize);
					wire[vertex2].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, OtherPos.y) * float(cellsize);
				} else {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(Pos.x, OtherPos.y) * float(cellsize);
				}
			} else {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(OtherPos.x, Pos.y) * float(cellsize);
				} else {
					wire[vertex1].position = sf::Vector2f(Pos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
					wire[vertex2].position = sf::Vector2f(OtherPos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
				}
			}
		} else if (Dir1 == Down) {
			if (Pos.x > OtherPos.x) {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = sf::Vector2f(Pos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
					wire[vertex2].position = sf::Vector2f(OtherPos.x, (Pos.y + OtherPos.y) / 2) * float(cellsize);
				} else {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(OtherPos.x, Pos.y) * float(cellsize);
				}
			} else {
				if (Pos.y < OtherPos.y) {
					wire[vertex1].position = wire[vertex2].position = sf::Vector2f(Pos.x, OtherPos.y) * float(cellsize);
				} else {
					wire[vertex1].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, Pos.y) * float(cellsize);
					wire[vertex2].position = sf::Vector2f((Pos.x + OtherPos.x) / 2, OtherPos.y) * float(cellsize);
				}
			}
		}
	}

	wire[0].color = wire[1].color = wire[2].color = wire[3].color
		= sf::Color::Black;
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
				pinShape.setFillColor(sf::Color::Yellow);
			} else {
				pinShape.setFillColor(sf::Color::Black);
			}
			sf::Vector2i relPos(static_cast<int>(pin["x"]),
				static_cast<int>(pin["y"]));
			relPos = ComponentInfo::rotatePin(pin);
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
	int width = component["width"].get<int>();
	int height = component["height"].get<int>();
	sf::RectangleShape shape(sf::Vector2f(
		static_cast<int>(width) * cellsize,
		static_cast<int>(height) * cellsize));
	shape.setRotation(component["rotation"].get<int>() * 90);
	shape.setTexture(
		&(TextureHolder::get()[component["texture"]]), true);
	sf::Vector2i position(
		static_cast<int>(component["position"]["x"]),
		static_cast<int>(component["position"]["y"]));
	switch (component["rotation"].get<int>()) {
	case 1:
		position.x += height;
		break;
	case 2:
		position += sf::Vector2i(width, height);
		break;
	case 3:
		position.y += width;
		break;
	}
	shape.setPosition(sf::Vector2f(position * cellsize));
	if (component.count("moving") == 1) {
		shape.setOutlineThickness(-1);
		if (QuadTree::get().intersects(ComponentInfo::getRect(component))
			== "") {
			shape.setOutlineColor(sf::Color(128, 255, 128));
			shape.setFillColor(sf::Color(128, 255, 128, 128));
		} else {
			shape.setOutlineColor(sf::Color::Red);
			shape.setFillColor(sf::Color(255, 0, 0, 128));
		}
	}
	App::get().window().draw(shape);
	m_drawPins(component);
}

