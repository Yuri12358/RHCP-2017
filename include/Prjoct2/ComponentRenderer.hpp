#ifndef COMPONENT_RENDERER_HPP
#define COMPONENT_RENDERER_HPP

#include<nlohmann/json.hpp>

class ComponentRenderer {
	ComponentRenderer();
	void m_drawConnection(nlohmann::json & pin);
	void m_drawPins(nlohmann::json & component);

	static ComponentRenderer * s_instance;

public:
	static ComponentRenderer & get();
	void drawCurrent();
	void drawComponent(nlohmann::json & component);
};

#endif // COMPONENT_RENDERER_HPP

