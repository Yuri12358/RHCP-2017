#include<Prjoct2/Utility.hpp>
#include<stdexcept>
#include<cctype>

std::string capitalize(std::string str) {
	str[0] = std::toupper(str[0]);
	return str;
}

std::string toString(float num) {
	std::string str = std::to_string(num);
	if (str.find('.') != std::string::npos) {
		while (str.back() == '0') {
			str.pop_back();
		}
		if (str.back() == '.') {
			str += '0';
		}
	}
	return str;
}

sf::Vector2f pairwiseDivide(sf::Vector2f a, sf::Vector2f b) {
	if (b.x == 0 || b.y == 0) {
		throw std::invalid_argument("pairwise ator division by 0");
	}
	return sf::Vector2f(a.x / b.x, a.y / b.y);
}

sf::Vector2f pairwiseMultiply(sf::Vector2f a, sf::Vector2f b) {
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}

