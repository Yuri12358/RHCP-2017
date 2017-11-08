#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include"Precomp.h"

#include<Prjoct2/State.hpp>
#include<SFML/Graphics.hpp>

class EditorState : public State {
	void m_handleKeyEvent(sf::Event::KeyEvent event);
	void m_handleZKeyPress(const sf::Event::KeyEvent & event);
	void m_handleMousePressEvent(sf::Event::MouseButtonEvent event);
	void m_updateScrolling();
	void m_updateMovingComponent();
	void m_renderBackground();
	void m_renderComponents();
	void m_placeNewComponent(int pressX, int pressY);
	void m_placeMovingComponent(int pressX, int pressY);
	void m_selectPin(int pressX, int pressY);
	void m_deselectPin();
	bool m_cancelStartedAction();
	void m_disconnectSelectedPin();
	void m_selectComponent(int x, int y);
	void m_connectPins(nlohmann::json & pin1, nlohmann::json & pin2);
	void m_disconnectPin(nlohmann::json & pin);

	sf::View m_fieldView;
	std::string m_bgTextureName;
	std::string m_currentCircuitName;
	int m_nextComponentID;
	bool m_locked;

public:
	typedef std::shared_ptr<EditorState> Ptr;

	EditorState(const std::string & bg = "");
	~EditorState();
	void handleEvent(sf::Event event);
	void render();
	void update();
	void updateFieldView();
	void cancelMovingComponent();
	void deleteSelectedComponent();
	void deleteComponent(const std::string & id);
	void rotateSelectedComponent();
	void moveSelectedComponent();
	void unlockUI();
	bool isUILocked();
	sf::Vector2f mapToFieldCoords(sf::Vector2i pixel);

	void createNewCircuit();
	void openCircuit();
	void openCircuit(const std::string & name);
	void saveCircuit();
	void saveCircuit(const std::string & name);
};

#endif // EDITOR_STATE_HPP

