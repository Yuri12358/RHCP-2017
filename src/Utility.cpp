#include<Prjoct2/Utility.hpp>
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

