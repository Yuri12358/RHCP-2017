#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP

#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<string>

class QuadTree {
	struct Node {
		enum Dir {
			UR,
			DR,
			DL,
			UL
		};

		Node();
		bool addObjectByRect(const std::string & id, sf::IntRect rect);
		bool removeObjectByRect(const std::string & id,
			sf::IntRect rect);
		std::string intersects(sf::IntRect rect);
		void dump(int depth = 0);
		static Dir getOpposite(Dir dir);
		static Dir getByCoords(bool x, bool y);

		sf::IntRect rect;
		Node * nodes[4];
		std::string componentID;
	
	private:
		void m_createChild(Dir dir);
	};

	QuadTree();
	void m_addObjectByRect(const std::string & id, sf::IntRect rect);
	void m_removeObjectByRect(const std::string & id, sf::IntRect rect);
	void m_init(sf::IntRect rect);
	void m_extend(sf::IntRect rect);
	void m_shrink();
	std::string m_getComponentID(int x, int y);

	static QuadTree * s_instance;
	Node * m_node;

public:
	static QuadTree & get();
	void addObject(const std::string & id);
	void removeObject(const std::string & id);
	std::string intersects(sf::IntRect rect);
	nlohmann::json * getPin(int x, int y);
	std::string getComponentID(sf::Vector2f position);
	void dump();
	void removeAll();
	void addAll();
};

#endif // QUAD_TREE_HPP

