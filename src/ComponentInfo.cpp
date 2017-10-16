#include<Prjoct2/ComponentInfo.hpp>
#include<Prjoct2/JSONHolder.hpp>

sf::IntRect ComponentInfo::getRect(const nlohmann::json & component) {
	int x = component["position"]["x"];
	int y = component["position"]["y"];
	int rotation = component["rotation"];
	int w, h;
	if (rotation % 2 == 0) {
		w = component["width"];
		h = component["height"];
	} else {
		w = component["height"];
		h = component["width"];
	}
	return sf::IntRect(x, y, w, h);
}

sf::IntRect ComponentInfo::getRect(const std::string & id) {
	nlohmann::json & component(JSONHolder::get()["components"][id]);
	return getRect(component);
}

nlohmann::json * ComponentInfo::getComponentPin(const std::string & id,
	int pinX, int pinY, bool withRotation) {
	nlohmann::json::iterator it = JSONHolder::get()["components"].find(id);
	if (it == JSONHolder::get()["components"].end()) {
		return nullptr;
	}
	if (withRotation) {
		int tx = pinX;
		int ty = pinY;
		switch ((*it)["rotation"].get<int>()) {
		case 1:
			pinX = ty;
			pinY = getRect(*it).width - tx;
			break;
		case 2:
			pinX = getRect(*it).width - tx;
			pinY = getRect(*it).height - ty;
			break;
		case 3:
			pinX = getRect(*it).height - ty;
			pinY = tx;
			break;
		}
	}
	std::string type = (*it)["type"];
	if (type == "dot") {
		nlohmann::json & pinPattern = (*it)["pin pattern"];
		if (pinX == static_cast<int>(pinPattern["x"])
			&& pinY == static_cast<int>(pinPattern["y"])) {
			int nextPinID = (*it)["next pin ID"];
			(*it)["next pin ID"] = nextPinID + 1;
			std::string strID(std::to_string(nextPinID));
			nlohmann::json & pin = (*it)["pins"][strID];
			pin = pinPattern;
			pin["parentID"] = id;
			pin["id"] = strID;
			return &pin;
		}
	} else {
		for (nlohmann::json & pin : (*it)["pins"]) {
			if (pinX == static_cast<int>(pin["x"])
				&& pinY == static_cast<int>(pin["y"])) {
				return &pin;
			}
		}
	}
	return nullptr;
}

nlohmann::json * ComponentInfo::getComponentPinAt(const std::string & id,
	int x, int y) {
	nlohmann::json::iterator it = JSONHolder::get()["components"]
		.find(id);
	if (it == JSONHolder::get()["components"].end()) {
		return nullptr;
	}
	int pinX = x - static_cast<int>((*it)["position"]["x"]);
	int pinY = y - static_cast<int>((*it)["position"]["y"]);
	return getComponentPin(id, pinX, pinY, true);
}

Direction ComponentInfo::getPinOrientation(const nlohmann::json & pin) {
	std::string parentID = pin["parentID"];
	const nlohmann::json & parent = JSONHolder::get()["components"]
		[parentID];
	int width = parent["width"];
	int height = parent["height"];
	Direction result = None;
	int x = static_cast<int>(pin["x"]);
	int y = static_cast<int>(pin["y"]);
	if (x == 0) {
		result = Direction(result | Left);
	}
	if (x == width) {
		result = Direction(result | Right);
	}
	if (y == 0) {
		result = Direction(result | Up);
	}
	if (y == height) {
		result = Direction(result | Down);
	}
	if (result == None) {
		result = All;
	}
	return rotateDirection(result, parent["rotation"].get<int>());
}

sf::Vector2i ComponentInfo::rotatePin(const nlohmann::json & pin) {
	nlohmann::json & component = JSONHolder::get()["components"]
		[pin["parentID"].get<std::string>()];
	sf::Vector2i relPos(pin["x"].get<int>(), pin["y"].get<int>());
	int tx = relPos.x;
	int ty = relPos.y;
	switch (component["rotation"].get<int>()) {
	case 3:
		relPos.x = ty;
		relPos.y = getRect(component).width - tx;
		break;
	case 2:
		relPos.x = getRect(component).width - tx;
		relPos.y = getRect(component).height - ty;
		break;
	case 1:
		relPos.x = getRect(component).height - ty;
		relPos.y = tx;
		break;
	}
	return relPos;
}

