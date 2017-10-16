#ifndef TEXTURE_HOLDER_HPP
#define TEXTURE_HOLDER_HPP

#include<SFML/Graphics.hpp>
#include<map>

class TextureHolder {
	TextureHolder();

	static TextureHolder * s_instance;
	std::map<std::string, sf::Texture> m_textures;
	
public:
	static TextureHolder & get();
	sf::Texture & operator[](const std::string & name);
	void set(const std::string & name, const sf::Texture & texture);
};

#endif // TEXTURE_HOLDER_HPP

