#ifndef APP_HPP
#define APP_HPP

#include<Prjoct2/Debug.hpp>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp> 
class App {
	App();
	void m_handleEvents();
	void m_handleMousePressEvent(const sf::Event::MouseButtonEvent & event);
	void m_handleMouseReleaseEvent(const sf::Event::MouseButtonEvent & e);
	void m_handleKeyEvent(const sf::Event::KeyEvent & event);
	void m_handleZKeyPress(const sf::Event::KeyEvent & event);
	void m_handleResizeEvent(const sf::Event::SizeEvent & event);

	void m_undo();
	void m_redo();
	
	void m_render();
	void m_renderBackground();
	void m_renderComponents();

	void m_update();
	void m_updateFieldView();
	void m_updateMovingComponent();

	void m_placeNewComponent(int pressX, int pressY);
	void m_placeMovingComponent(int pressX, int pressY);

	void m_selectPin(int pressX, int pressY);
	void m_selectComponent(int x, int y);

	void m_deselectPin();

	void m_connectPins(nlohmann::json & pin1, nlohmann::json & pin2);

	void m_disconnectPin(nlohmann::json & pin);
	void m_disconnectSelectedPin();

	bool m_cancelStartedAction();

	static App * s_instance;
	sf::RenderWindow m_window;
	sf::View m_fieldView;
	sf::View m_defaultView;
	std::string m_currentCircuitName;
	int m_nextComponentID;
	bool m_locked;

public:
	static App & get();
	void run();
	void setFieldView();
	void resetView();
	sf::RenderWindow & window();
	sf::Vector2f mapToFieldCoords(sf::Vector2i pixel);
	void deleteSelectedComponent();
	void deleteComponent(const std::string & id);
	void moveSelectedComponent();
	void rotateSelectedComponent();
	void cancelMovingComponent();

	void createNewCircuit();
	void openCircuit();
	void openCircuit(const std::string & name);
	void saveCircuit();
	void saveCircuit(const std::string & name);

	void unlockUI();
};

#endif // APP_HPP

