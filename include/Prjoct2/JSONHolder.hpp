#ifndef JSON_HOLDER_HPP
#define JSON_HOLDER_HPP

#include<nlohmann/json.hpp>
#include<map>

class JSONHolder {
	JSONHolder();

	static JSONHolder * s_instance;
	std::map<std::string, nlohmann::json> m_jsons;

public:
	static JSONHolder & get();
	nlohmann::json & operator[](const std::string & name);
	nlohmann::json & fromFile(const std::string & filename);
	void save(const std::string & name);
};

#endif // JSON_HOLDER_HPP

