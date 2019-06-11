#include "MazeNode.h"


MazeNode::MazeNode(int x_coord, int y_coord) {
	x = x_coord;
	y = y_coord;

}

int MazeNode::isNodeStart() {
	return isStart;
}

int MazeNode::isNodeFinish() {
	return isFinish;
}

int MazeNode::isLeftTurn() {
	return left;
}

int MazeNode::isRightTurn() {
	return right;
}

int MazeNode::isStraightTurn() {
	return straight;
}

int MazeNode::isBackTurn() {
	return back;
}

void MazeNode::hasLeftTurn() {
	left = 1;
}

void MazeNode::hasRightTurn() {
	right = 1;
}

void MazeNode::hasStraight() {
	straight = 1;
}

void MazeNode::hasBack() {
	back = 1;
}

void MazeNode::setFinish() {
	isFinish = 1;
}
void MazeNode::setStart() {
	isStart = 1;
}
