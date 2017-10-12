#include<Prjoct2/History.hpp>

History * History::s_instance = nullptr;

History::History() {
}

History & History::get() {
	if (s_instance == nullptr) {
		s_instance = new History();
	}
	return *(s_instance);
}

