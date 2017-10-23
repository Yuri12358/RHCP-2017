#ifndef STATE_HPP
#define STATE_HPP

#include<SFML/Window.hpp>
#include<memory>

class State {
public:
	typedef std::shared_ptr<State> Ptr;

	virtual void handleEvent(sf::Event event) = 0;
	virtual void render() = 0;
	virtual void update() = 0;
};

#endif // STATE_HPP

