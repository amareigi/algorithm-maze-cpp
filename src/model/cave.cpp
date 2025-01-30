#include "cave.hpp"

void Cave::initField(CaveData& caveData, int rows, int cols) {
  std::vector<int> cap(cols);
  caveData.caveSize.push_back(rows);
  caveData.caveSize.push_back(cols);
  for (int i = 0; i < rows; ++i) {
    caveData.field.push_back(cap);
  }
}

void Cave::fillRandomCells(CaveData& caveData, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int chance = rand() % 100;
      chance += caveData.startChance;
      caveData.field[i][j] = chance > 100 ? 1 : 0;
    }
  }
};

void Cave::clearCave(CaveData& caveData) {
  caveData.caveSize.clear();
  caveData.field.clear();
};

void Cave::fillFieldFromFile(CaveData& caveData, const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw "Error";
  }

  std::string line;
  int line_counter = 0;

  while (std::getline(file, line)) {
    int num;
    std::vector<int> temp;
    std::istringstream iss(line);

    if (line_counter == 0) {
      for (int i = 0; i < 2; ++i) {
        iss >> num;
        caveData.caveSize.push_back(num);
      }
    } else {
      while (iss >> num) temp.push_back(num);

      if (line_counter < caveData.caveSize[0] + 1) {
        caveData.field.push_back(temp);
      }
    }
    line_counter++;
  }
  file.close();
};

void Cave::saveToFile(const CaveData& caveData, const std::string& filename) {
  std::ofstream file(filename);
  file << caveData.caveSize[0] << " " << caveData.caveSize[1] << '\n';

  for (auto row : caveData.field) {
    for (auto el : row) {
      file << el << " ";
    }
    file << '\n';
  }
  file.close();
};

void Cave::generateCave(CaveData& caveData, int rows, int cols) {
  srand(time(0));
  initField(caveData, rows, cols);
  fillRandomCells(caveData, rows, cols);
};

bool Cave::formalization(CaveData& caveData) {
  int rows = caveData.caveSize[0];
  int cols = caveData.caveSize[1];
  int temp[rows][cols];
  bool res = false;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      temp[i][j] = caveData.field[i][j];
    }
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int k = (i + 1 == rows) ? i : (i + 1);
      int l = (i - 1 < 0) ? i : (i - 1);
      int m = (j + 1 == cols) ? j : (j + 1);
      int n = (j - 1 < 0) ? j : (j - 1);

      int count = 0;

      if (k == i || caveData.field[k][j] == 1) count++;
      if (l == i || caveData.field[l][j] == 1) count++;
      if (m == j || caveData.field[i][m] == 1) count++;
      if (n == j || caveData.field[i][n] == 1) count++;
      if (k == i || m == j || caveData.field[k][m] == 1) count++;
      if (k == i || n == j || caveData.field[k][n] == 1) count++;
      if (l == i || m == j || caveData.field[l][m] == 1) count++;
      if (l == i || n == j || caveData.field[l][n] == 1) count++;

      if (caveData.field[i][j] == 1 && count < caveData.limitDie) {
        temp[i][j] = 0;
      }
      if (caveData.field[i][j] == 0 && count > caveData.limitBorn) {
        temp[i][j] = 1;
      }
    }
  }
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (!res && caveData.field[i][j] != temp[i][j]) res = true;
      caveData.field[i][j] = temp[i][j];
    }
  }
  return res;
};