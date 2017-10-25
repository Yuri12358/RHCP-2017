#ifndef UTILITY_HPP
#define UTILITY_HPP

#include<SFML/System/Vector2.hpp>
#include<iostream>

template <typename T>
std::ostream & operator<<(std::ostream & os, sf::Vector2<T> vec);

std::string capitalize(std::string str);

std::string toString(float num);

sf::Vector2f pairwiseDivide(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f pairwiseMultiply(sf::Vector2f a, sf::Vector2f b);

#include<Prjoct2/Utility.inl>

#endif // UTILITY_HPP

