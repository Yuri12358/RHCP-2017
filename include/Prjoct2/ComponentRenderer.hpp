#ifndef COMPONENT_RENDERER_HPP
#define COMPONENT_RENDERER_HPP

#include"Precomp.h"

#include<Prjoct2/Direction.hpp>
#include<SFML/Graphics.hpp>

class ComponentRenderer {
	ComponentRenderer();
	void m_drawWire(nlohmann::json & pin);
	void m_drawPins(nlohmann::json & component);

	bool m_tryConnectionWithDot(Direction dir1, Direction dir2,
		sf::VertexArray & wire);
	bool m_tryHorizontalOppositeConnection(Direction dir1, Direction dir2,
		sf::VertexArray & wire);
	bool m_tryVerticalOppositeConnection(Direction dir1, Direction dir2,
		sf::VertexArray & wire);
	bool m_tryHorizontalCodirectionalConnection(Direction dir1,
		Direction dir2, sf::VertexArray & wire);
	bool m_tryVerticalCodirectionalConnection(Direction dir1,
		Direction dir2, sf::VertexArray & wire);
	bool m_tryCornerConnection(Direction dir1, Direction dir2,
		sf::VertexArray & wire);

	static ComponentRenderer * s_instance;

public:
	static ComponentRenderer & get();
	void drawCurrent();
	void drawComponent(nlohmann::json & component);
};

#endif // COMPONENT_RENDERER_HPP

