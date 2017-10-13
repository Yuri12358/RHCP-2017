#include<Prjoct2/QuadTree.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/App.hpp>
#include<Prjoct2/ComponentInfo.hpp>
#include<iostream>
#include<cassert>

QuadTree * QuadTree::s_instance = nullptr;

QuadTree::Node::Node() {
	nodes[UL] = nodes[UR] = nodes[DL] = nodes[DR] = nullptr;
}

QuadTree::Node::Dir QuadTree::Node::getOpposite(QuadTree::Node::Dir dir) {
	switch (dir) {
	case UR:
		return DL;
	case DR:
		return UL;
	case DL:
		return UR;
	case UL:
		return DR;
	}
}

QuadTree::Node::Dir QuadTree::Node::getByCoords(bool x, bool y) {
	if (x && y) {
		return DR;
	} else if (x) {
		return UR;
	} else if (y) {
		return DL;
	} else {
		return UL;
	}
}

std::string QuadTree::Node::intersects(sf::IntRect rect) {
	sf::IntRect result;
	if (!rect.intersects(this->rect, result)) {
		return "";
	}
	if (componentID != "") {
		return componentID;
	}
	for (int i = 0; i < 4; ++i) {
		Dir dir = (Dir) i;
		if (nodes[dir] != nullptr) {
			std::string id = nodes[dir]->intersects(rect);
			if (id != "") {
				return id;
			}
		}
	}
	return "";
}

bool QuadTree::Node::addObjectByRect(const std::string & id, sf::IntRect rect) {
	sf::IntRect result;
	if (!rect.intersects(this->rect, result)) {
		return false;
	}
	if (result == this->rect) {
		componentID = id;
		return true;
	}
	for (int i = 0; i < 4; ++i) {
		Dir dir = (Dir) i;
		if (nodes[dir] == nullptr) {
			m_createChild(dir);
			if (!(nodes[dir]->addObjectByRect(id, rect))) {
				delete nodes[dir];
				nodes[dir] = nullptr;
			}
		} else {
			nodes[dir]->addObjectByRect(id, rect);
		}
	}
	return true;
}

bool QuadTree::Node::removeObjectByRect(const std::string & id, sf::IntRect rect) {
	sf::IntRect result;
	if (!rect.intersects(this->rect, result)) {
		return false;
	}
	if (result == this->rect && componentID == id) {
		componentID = "";
		return true;
	}
	int childLeft = 4;
	for (int i = 0; i < 4; ++i) {
		Dir dir = (Dir) i;
		if (nodes[dir] != nullptr) {
			if (nodes[dir]->removeObjectByRect(id, rect)) {
				delete nodes[dir];
				nodes[dir] = nullptr;
				--childLeft;
			}
		} else {
			--childLeft;
		}
	}
	return childLeft == 0;
}

void QuadTree::Node::m_createChild(QuadTree::Node::Dir dir) {
	assert(nodes[dir] == nullptr);
	assert(rect.width > 1 && rect.height > 1);
	nodes[dir] = new Node();
	nodes[dir]->rect = rect;
	nodes[dir]->rect.width /= 2;
	nodes[dir]->rect.height /= 2;
	switch (dir) {
	case UR:
		nodes[dir]->rect.left += nodes[dir]->rect.width;
		break;
	case DR:
		nodes[dir]->rect.left += nodes[dir]->rect.width;
		nodes[dir]->rect.top += nodes[dir]->rect.height;
		break;
	case DL:
		nodes[dir]->rect.top += nodes[dir]->rect.height;
		break;
	}
}

QuadTree::QuadTree()
	: m_node() {
}

QuadTree & QuadTree::get() {
	if (s_instance == nullptr) {
		s_instance = new QuadTree();
	}
	return *s_instance;
}

void QuadTree::addObject(const std::string & id) {
	if (JSONHolder::get()["components"].count(id) == 1) {
		auto rect = ComponentInfo::getRect(id);
		m_addObjectByRect(id, rect);
	}
}

void QuadTree::removeObject(const std::string & id) {
	if (JSONHolder::get()["components"].count(id) == 1) {
		auto rect = ComponentInfo::getRect(id);
		m_removeObjectByRect(id, rect);
	}
}

void QuadTree::m_removeObjectByRect(const std::string & id, sf::IntRect rect) {
	if (m_node == nullptr) {
		return;
	}
	sf::IntRect result;
	rect.intersects(m_node->rect, result);
	if (m_node->removeObjectByRect(id, rect)) {
		delete m_node;
		m_node = nullptr;
	} else {
		m_shrink();
	}
}

void QuadTree::m_shrink() {
	bool needShrink = true;
	while (needShrink) {
		int lastNonNull = -1;
		int count = 0;
		for (int i = 0; i < 4; ++i) {
			if (m_node->nodes[i] != nullptr) {
				++count;
				lastNonNull = i;
			}
		}
		if (count == 1) {
			Node * newRoot = m_node->nodes[lastNonNull];
			delete m_node;
			m_node = newRoot;
			needShrink = true;
		} else {
			needShrink = false;
		}
	}
}

void QuadTree::m_addObjectByRect(const std::string & id, sf::IntRect rect) {
	if (m_node == nullptr) {
		m_init(rect);
	}
	sf::IntRect result;
	rect.intersects(m_node->rect, result);
	while (result != rect) {
		m_extend(rect);
		rect.intersects(m_node->rect, result);
	}
	m_node->addObjectByRect(id, rect);
}

void QuadTree::m_extend(sf::IntRect rect) {
	bool toRight = rect.left >= m_node->rect.left;
	bool toBottom = rect.top >= m_node->rect.top;
	Node::Dir extendTo = Node::getByCoords(toRight, toBottom);
	Node * newNode = new Node();
	newNode->rect = m_node->rect;
	switch (extendTo) {
	case Node::UR:
		newNode->rect.top -= m_node->rect.height;
		break;
	case Node::UL:
		newNode->rect.top -= m_node->rect.height;
		newNode->rect.left -= m_node->rect.width;
		break;
	case Node::DL:
		newNode->rect.left -= m_node->rect.width;
		break;
	}
	newNode->rect.width *= 2;
	newNode->rect.height *= 2;
	newNode->nodes[Node::getOpposite(extendTo)] = m_node;
	m_node = newNode;
}

void QuadTree::m_init(sf::IntRect rect) {
	int max = std::max(rect.width, rect.height);
	int powerOf2 = 1;
	while (powerOf2 < max) {
		powerOf2 *= 2;
	}
	rect.width = rect.height = powerOf2;
	m_node = new Node();
	m_node->rect = rect;
}

std::string QuadTree::intersects(sf::IntRect rect) {
	if (m_node != nullptr) {
		return m_node->intersects(rect);
	}
	return "";
}

std::string QuadTree::m_getComponentID(int x, int y) {
	return intersects(sf::IntRect(x, y, 1, 1));
}

nlohmann::json * QuadTree::getPin(int x, int y) {
	nlohmann::json * pin = ComponentInfo::getComponentPinAt(
				m_getComponentID(x - 1, y - 1), x, y);
	if (pin != nullptr) {
		return pin;
	}
	pin = ComponentInfo::getComponentPinAt(
			m_getComponentID(x, y - 1), x, y);
	if (pin != nullptr) {
		return pin;
	}
	pin = ComponentInfo::getComponentPinAt(
		m_getComponentID(x - 1, y), x, y);
	if (pin != nullptr) {
		return pin;
	}
	pin = ComponentInfo::getComponentPinAt(
		m_getComponentID(x, y), x, y);
	return pin;
}

std::string QuadTree::getComponentID(sf::Vector2f position) {
	 int cellsize = JSONHolder::get()["settings"]["cellsize"];
	 sf::Vector2i iPosition(position / float(cellsize));
	 return m_getComponentID(iPosition.x, iPosition.y);
}

void QuadTree::dump() {
	if (m_node == nullptr) {
		std::cout << "null";
	} else {
		m_node->dump();
	}
	std::cout << std::endl;
}

void QuadTree::Node::dump(int depth) {
	auto addSpace = [&](int d) {
		for (int i = 0; i < d; ++i) {
			std::cout << "    ";
		}
	};
	std::cout << "{\n";
	addSpace(depth + 1);
	std::cout << "rect: (" << rect.left << ", " << rect.top << ", "
		<< rect.width << ", " << rect.height << ")\n";
	if (componentID != "") {
		addSpace(depth + 1);
		std::cout << "component id: " << componentID << '\n';
	}
	for (int i = 0; i < 4; ++i) {
		if (nodes[i] != nullptr) {
			addSpace(depth + 1);
			switch (i) {
			case 0:
				std::cout << "UR: ";
				break;
			case 1:
				std::cout << "DR: ";
				break;
			case 2:
				std::cout << "DL: ";
				break;
			case 3:
				std::cout << "UL: ";
			}
			nodes[i]->dump(depth + 1);
		}
	}
	addSpace(depth);
	std::cout << "}\n";
}

void QuadTree::removeAll() {
	for (nlohmann::json & component : JSONHolder::get()["components"]) {
		removeObject(component["id"].get<std::string>());
	}
}

void QuadTree::addAll() {
	for (nlohmann::json & component : JSONHolder::get()["components"]) {
		addObject(component["id"].get<std::string>());
	}
}

