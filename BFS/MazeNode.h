#ifndef MazeNode_h
#define MazeNode_h

class MazeNode {
 private:
  int x = 0;
  int y = 0;
  int isStart = 0;
  int isFinish = 0;
  int left = 0;
  int right = 0;
  int straight = 0;
  int back = 0;
 public:
  MazeNode(){};
  MazeNode(int st_x, int st_y);
  int isNodeStart();
  int isNodeFinish();
  int isLeftTurn();
  int isRightTurn();
  int isStraightTurn();
  int isBackTurn();
  void hasLeftTurn();
  void hasRightTurn();
  void hasStraight();
  void hasBack();
  void setFinish();
  void setStart();
};

#endif
