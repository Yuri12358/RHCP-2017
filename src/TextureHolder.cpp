#include<Prjoct2/TextureHolder.hpp>
#include<stdexcept>

const std::string texturePath = "data/textures/";

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
		std::string fullname = texturePath + name + ".png";
		if (!m_textures[name].loadFromFile(fullname)) {
			throw std::runtime_error("Failed to load texture: '"
				+ name + "'");
		}
	}
	return m_textures[name];
}

