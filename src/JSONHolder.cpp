#include<Prjoct2/JSONHolder.hpp>
#include<fstream>
#include<stdexcept>
#include<iomanip>

const std::string confPath = "data/conf/";

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

nlohmann::json & JSONHolder::fromFile(const std::string & filename) {
	std::string fullname = confPath + filename + ".json";
	std::ifstream file(fullname);
	if (!file.is_open()) {
		throw std::runtime_error("JSON not found: '" + fullname + "'");
	}
	file >> m_jsons[filename];
	return m_jsons[filename];
}

void JSONHolder::save(const std::string & name) {
	std::string fullname = confPath + name + ".json";
	std::ofstream file(fullname);
	file << std::setw(4) << m_jsons[name];
}

