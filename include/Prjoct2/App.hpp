#ifndef APP_HPP
#define APP_HPP

#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>

class App {
	App();
	void m_handleEvents();
	void m_handleMousePressEvent(const sf::Event::MouseButtonEvent & event);
	void m_handleMouseReleaseEvent(const sf::Event::MouseButtonEvent & e);
	void m_handleKeyEvent(const sf::Event::KeyEvent & event);
	void m_render();
	void m_renderComponents();
	void m_update();
	void m_updateFieldView();
	void m_placeNewComponent(int pressX, int pressY);
	void m_selectPin(int pressX, int pressY);
	void m_connectPins(nlohmann::json & pin1, nlohmann::json & pin2);
	void m_disconnectPin(nlohmann::json & pin);
	void m_selectComponent(int x, int y);

	static App * s_instance;
	sf::RenderWindow m_window;
	sf::View m_fieldView;
	int m_nextComponentID;

public:
	static App & get();
	void run();
	void setFieldView();
	void resetView();
	sf::RenderWindow & window();
	sf::Vector2f mapToFieldCoords(sf::Vector2i pixel);
	void deleteSelectedComponent();
};

#endif // APP_HPP
