#ifndef S21_CAVE_HPP
#define S21_CAVE_HPP

#include "../common.h"

struct Cave {
  void initField(CaveData& caveData, int rows, int cols);
  void fillRandomCells(CaveData& caveData, int rows, int cols);
  void generateCave(CaveData& caveData, int rows, int cols);
  bool formalization(CaveData& caveData);
  void clearCave(CaveData& caveData);
  void saveToFile(const CaveData& caveData, const std::string& filename);
  void fillFieldFromFile(CaveData& caveData, const std::string& filename);
};

#endif  // S21_CAVE_HPP
