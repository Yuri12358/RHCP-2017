#ifndef UTILITY_HPP
#define UTILITY_HPP

#include<SFML/System/Vector2.hpp>
#include<iostream>

template <typename T>
std::ostream & operator<<(std::ostream & os, sf::Vector2<T> vec);

#include<Prjoct2/Utility.inl>

#endif // UTILITY_HPP

