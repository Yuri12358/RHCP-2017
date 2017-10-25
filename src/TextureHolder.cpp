#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<stdexcept>

TextureHolder * TextureHolder::s_instance = nullptr;

TextureHolder::TextureHolder() {
}

TextureHolder & TextureHolder::get() {
	if (s_instance == nullptr) {
		s_instance = new TextureHolder();
	}
	return *s_instance;
}

sf::Texture & TextureHolder::operator[](const std::string & name) {
	if (m_textures.count(name) == 0) {
		std::string fullname = "data/resources/" + JSONHolder::get()
			["settings"]["resource pack"].get<std::string>()
			+ "/textures/" + name + ".png";
		if (!m_textures[name].loadFromFile(fullname)) {
			throw std::runtime_error("Failed to load texture: '"
				+ name + "'");
		}
		m_textures[name].setSmooth(true);
	}
	return m_textures[name];
}

void TextureHolder::set(const std::string & name, const sf::Texture & value) {
	m_textures[name] = value;
	m_textures[name].setSmooth(true);
}

