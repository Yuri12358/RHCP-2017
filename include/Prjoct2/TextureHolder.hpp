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
};

#endif // TEXTURE_HOLDER_HPP

