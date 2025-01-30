#include "controller.hpp"

void Controller::findWay(Data& data, way_t& way, const Coord_t& start,
                         const Coord_t& finish) {
  M.findWay(data, way, start, finish);
};

void Controller::loadFile(Data& data, const std::string& filename) {
  M.fillFieldFromFile(data, filename);
};

void Controller::saveFile(const Data& data, const std::string& filename) {
  M.saveToFile(data, filename);
};

void Controller::generate(Data& data, int rows, int cols) {
  M.generateMaze(data, rows, cols);
};

void Controller::clearMazeData(Data& data, way_t& way) { M.clear(data, way); }

void Controller::clearCaveData(CaveData& caveData) { C.clearCave(caveData); }

void Controller::generateCave(CaveData& caveData, int rows, int cols) {
  C.generateCave(caveData, rows, cols);
};

void Controller::loadCaveFile(CaveData& caveData, const std::string& filename) {
  C.fillFieldFromFile(caveData, filename);
};

void Controller::saveCaveFile(const CaveData& caveData,
                              const std::string& filename) {
  C.saveToFile(caveData, filename);
};

bool Controller::formalization(CaveData& caveData) {
  return C.formalization(caveData);
};