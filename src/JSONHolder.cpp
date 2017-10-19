#include<Prjoct2/JSONHolder.hpp>
#include<fstream>
#include<stdexcept>
#include<iomanip>

JSONHolder * JSONHolder::s_instance = nullptr;

JSONHolder::JSONHolder() {
	fromFile("settings");
}

JSONHolder & JSONHolder::get() {
	if (s_instance == nullptr) {
		s_instance = new JSONHolder();
	}
	return *s_instance;
}

nlohmann::json & JSONHolder::operator[](const std::string & name) {
	return m_jsons[name];
}

nlohmann::json & JSONHolder::fromFile(const std::string & filename,
	bool useStandartPrefix, const std::string & name) {
	std::string fullname = (useStandartPrefix ? "data/conf/" : "")
		+ filename + ".json";
	std::ifstream file(fullname);
	if (!file.is_open()) {
		throw std::runtime_error("JSON not found: '" + fullname + "'");
	}
	std::string realName = (name == "" ? filename : name);
	file >> m_jsons[realName];
	return m_jsons[realName];
}

void JSONHolder::save(const std::string & name) {
	std::string fullname = "data/conf/" + name + ".json";
	std::ofstream file(fullname);
	file << std::setw(4) << m_jsons[name];
}

