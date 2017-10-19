template <typename T>
std::ostream & operator<<(std::ostream & os, sf::Vector2<T> vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
}

