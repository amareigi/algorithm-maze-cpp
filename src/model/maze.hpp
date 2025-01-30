#ifndef S21_MAZE_HPP
#define S21_MAZE_HPP

#include "../common.h"

struct Maze {
  void fillFieldFromFile(Data& data, const std::string& filename);
  void saveToFile(const Data& data, const std::string& filename);
  void generateMaze(Data& data, int rows, int cols);
  void clear(Data& data, way_t& way);
  void setRightWalls(Data& data, std::vector<int>& set, int row, int cols);
  void setBottomWalls(Data& data, const std::vector<int>& set, int row,
                      int cols);
  void findWay(Data& data, way_t& way, const Coord_t& start,
               const Coord_t& finish);

  void fillGenData(Data& data, int rows, int cols);
  void uniteSets(std::vector<int>& set, int cols, int dest);
  void newLineInit(const Data& data, std::vector<int>& set, int row, int cols);
  void finalLine(Data& data, std::vector<int>& set, int row, int cols);
  void checkNextStep(const Data& data, way_t& way);
  bool isFinish(const Coord_t& finish);
  void initWasHereField(Data& data);
  void step(way_t& way);

  Coord_t curPos;
  way_t variants;
  way_t crossroads;
  std::vector<std::vector<bool>> wasHere;

 private:
};

#endif  // S21_MAZE_HPP