#ifndef HISTORY_HPP
#define HISTORY_HPP

#include<list>
#include<memory>

class History {
	struct Node {
	};

	History();

	static History * s_instance;
	std::list<std::unique_ptr<Node> > m_list;

public:
	static History & get();
};

#endif // HISTORY_HPP

