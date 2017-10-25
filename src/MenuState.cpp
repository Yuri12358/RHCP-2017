#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/MenuState.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>

MenuState::MenuState(const std::string & data)
	: m_data(data) {
}

bool MenuState::m_checkTexturePartHover(int l, int t, int r, int b) {
	sf::Vector2f size(TextureHolder::get()["mainMenu"].getSize());
	sf::Vector2f ulCorner(l, t);
	sf::Vector2f drCorner(r, b);
	sf::FloatRect normalButtonRect(pairwiseDivide(ulCorner, size),
		pairwiseDivide(drCorner - ulCorner, size));
	sf::Vector2f normalMouse(pairwiseDivide(sf::Vector2f(sf::Mouse
		::getPosition(App::get().window())), sf::Vector2f(App::get()
		.window().getSize())));
	return normalButtonRect.contains(normalMouse);
}

