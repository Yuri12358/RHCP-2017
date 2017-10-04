#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum Direction {
	None		= 0,
	Up		= (1 << 0),
	Right		= (1 << 1),
	Down		= (1 << 2),
	Left		= (1 << 3),
	All		= (Up | Right | Down | Left)
};

Direction getOppositeDirection(Direction dir);
Direction rotateDirection(Direction dir, int times);
bool checkCollinearity(Direction dir1, Direction dir2);
bool checkPerpendicularity(Direction dir1, Direction dir2);

#endif // DIRECTION_HPP

