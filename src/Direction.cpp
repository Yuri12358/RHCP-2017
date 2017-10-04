#include<Prjoct2/Direction.hpp>

Direction getOppositeDirection(Direction dir) {
	return Direction(((dir << 2) | (dir >> 2)) & 0xf);
}

Direction rotateDirection(Direction dir, int times) {
	times %= 4;
	if (dir == None || dir == All || times == 0) {
		return dir;
	}
	if (times > 0) {
		switch (dir) {
		case Up:
			return rotateDirection(Right, times - 1);
		case Right:
			return rotateDirection(Down, times - 1);
		case Down:
			return rotateDirection(Left, times - 1);
		case Left:
			return rotateDirection(Up, times - 1);
		}
	}
	switch (dir) {
	case Up:
		return rotateDirection(Left, times + 1);
	case Right:
		return rotateDirection(Up, times + 1);
	case Down:
		return rotateDirection(Right, times + 1);
	case Left:
		return rotateDirection(Down, times + 1);
	}
}

bool checkCollinearity(Direction dir1, Direction dir2) {
	return (dir1 & dir2) || (dir1 & getOppositeDirection(dir2));
}

bool checkPerpendicularity(Direction dir1, Direction dir2) {
	return (dir1 & rotateDirection(dir2, -1))
		|| (dir1 & rotateDirection(dir2, 1));
}

