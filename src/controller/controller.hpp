#ifndef S21_CONTROLLER_HPP
#define S21_CONTROLLER_HPP

#include "../model/cave.hpp"
#include "../model/maze.hpp"

struct Controller {
  Cave C;
  Maze M;

  // Maze methods
  void loadFile(Data& data, const std::string& filename);
  void saveFile(const Data& data, const std::string& filename);
  void generate(Data& data, int rows, int cols);
  void findWay(Data& data, way_t& way, const Coord_t& start,
               const Coord_t& finish);
  void clearMazeData(Data& data, way_t& way);

  // Cave methods
  void generateCave(CaveData& caveData, int rows, int cols);
  void loadCaveFile(CaveData& caveData, const std::string& filename);
  void saveCaveFile(const CaveData& caveData, const std::string& filename);
  bool formalization(CaveData& caveData);
  void clearCaveData(CaveData& caveData);
};

#endif  // S21_CONTROLLER_HPP
