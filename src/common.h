#ifndef S21_COMMON_MAZE_H
#define S21_COMMON_MAZE_H

#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Coord_t {
  int x;
  int y;
};

struct Data {
  std::vector<int> mazeSize;
  std::vector<std::vector<int>> rightWalls;
  std::vector<std::vector<int>> bottomWalls;
};

struct CaveData {
  int startChance;
  int limitBorn;
  int limitDie;
  std::vector<int> caveSize;
  std::vector<std::vector<int>> field;
};

using way_t = std::vector<Coord_t>;

#endif  // S21_COMMON_MAZE_H