#include "maze.hpp"

void Maze::clear(Data& data, way_t& way) {
  data.mazeSize.clear();
  data.rightWalls.clear();
  data.bottomWalls.clear();
  way.clear();
}

void Maze::fillGenData(Data& data, int rows, int cols) {
  std::vector<int> cap(cols);
  data.mazeSize.push_back(rows);
  data.mazeSize.push_back(cols);
  for (int i = 0; i < rows; ++i) {
    data.rightWalls.push_back(cap);
    data.bottomWalls.push_back(cap);
  }
}

void Maze::uniteSets(std::vector<int>& set, int cols, int dest) {
  int comprSet = set[dest + 1];
  for (int k = 0; k < cols; k++) {
    if (set[k] == comprSet) set[k] = set[dest];
  }
}

void Maze::setRightWalls(Data& data, std::vector<int>& set, int row, int cols) {
  for (int j = 0; j < cols - 1; j++) {
    int solution = rand() % 2;

    if (solution || set[j] == set[j + 1]) {
      data.rightWalls[row][j] = 1;
    } else {
      uniteSets(set, cols, j);
    }
  }
};

void Maze::setBottomWalls(Data& data, const std::vector<int>& set, int row,
                          int cols) {
  for (int j = 0; j < cols; j++) {
    int solution = rand() % 2;

    if (solution) {
      int count = 0;

      for (int k = 0; k < cols; k++) {
        if (set[k] == set[j] && data.bottomWalls[row][k] == 0) count++;
      }
      if (count > 1) data.bottomWalls[row][j] = 1;
    }
  }
};

void Maze::newLineInit(const Data& data, std::vector<int>& set, int row,
                       int cols) {
  for (int j = 0; j < cols; j++) {
    if (data.bottomWalls[row - 1][j] == 1) {
      set[j] = j + cols * row * 10;
    }
  }
}

void Maze::finalLine(Data& data, std::vector<int>& set, int row, int cols) {
  for (int j = 0; j < cols - 1; j++) {
    if (set[j] != set[j + 1]) {
      data.rightWalls[row][j] = 0;
      uniteSets(set, cols, j);
    }
  }
}

void Maze::generateMaze(Data& data, int rows, int cols) {
  fillGenData(data, rows, cols);
  std::vector<int> set;
  srand(time(0));

  for (int j = 0; j < cols; j++) {
    set.push_back(j);
  }

  for (int i = 0; i < rows; i++) {
    if (i) newLineInit(data, set, i, cols);
    setRightWalls(data, set, i, cols);
    if (i != rows - 1) setBottomWalls(data, set, i, cols);
    if (i == rows - 1) finalLine(data, set, i, cols);
  }
};

void Maze::fillFieldFromFile(Data& data, const std::string& filename) {
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
        data.mazeSize.push_back(num);
      }
    } else {
      while (iss >> num) temp.push_back(num);

      if (line_counter < data.mazeSize[0] + 1) {
        data.rightWalls.push_back(temp);
      } else if ((line_counter < data.mazeSize[0] * 2 + 2) &&
                 (line_counter > data.mazeSize[0] + 1)) {
        data.bottomWalls.push_back(temp);
      }
    }
    line_counter++;
  }
  file.close();
};

void Maze::saveToFile(const Data& data, const std::string& filename) {
  std::ofstream file(filename);
  file << data.mazeSize[0] << " " << data.mazeSize[1] << '\n';

  for (int i = 0; i < 2; ++i) {
    for (auto row : (i ? data.bottomWalls : data.rightWalls)) {
      for (auto el : row) {
        file << el << " ";
      }
      file << '\n';
    }
    i || (file << '\n');
  }
  file.close();
};

void Maze::initWasHereField(Data& data) {
  std::vector<bool> cap(data.mazeSize[1], false);
  for (int i = 0; i < data.mazeSize[0]; ++i) {
    wasHere.push_back(cap);
  }
}

void Maze::checkNextStep(const Data& data, way_t& way) {
  int row = curPos.y;
  int col = curPos.x;
  int rows = data.mazeSize[0];
  int cols = data.mazeSize[1];
  int wayCount{};
  // смотрим назад
  if (col > 0 && !data.rightWalls[row][col - 1] && !wasHere[row][col - 1]) {
    variants.push_back({col - 1, row});
    wayCount++;
  }
  // смотрим вверх
  if (row > 0 && !data.bottomWalls[row - 1][col] && !wasHere[row - 1][col]) {
    variants.push_back({col, row - 1});
    wayCount++;
  }
  // смотрим вперед
  if (col < cols - 1 && !data.rightWalls[row][col] && !wasHere[row][col + 1]) {
    variants.push_back({col + 1, row});
    wayCount++;
  }
  // смотрим вниз
  if (row < rows - 1 && !data.bottomWalls[row][col] && !wasHere[row + 1][col]) {
    variants.push_back({col, row + 1});
    wayCount++;
  }
  while (wayCount == 0 && (way.back().x != crossroads.back().x ||
                           way.back().y != crossroads.back().y)) {
    way.pop_back();
  }
  if (wayCount == 0) crossroads.pop_back();
  while ((wayCount--) > 1) crossroads.push_back(curPos);
}

void Maze::step(way_t& way) {
  wasHere[curPos.y][curPos.x] = true;
  Coord_t nextPos = variants.back();
  variants.pop_back();
  way.push_back(nextPos);
  curPos.x = nextPos.x;
  curPos.y = nextPos.y;
}

bool Maze::isFinish(const Coord_t& finish) {
  if (curPos.x == finish.x && curPos.y == finish.y) return true;
  return false;
};

void Maze::findWay(Data& data, way_t& way, const Coord_t& start,
                   const Coord_t& finish) {
  way.clear();
  wasHere.clear();
  variants.clear();
  crossroads.clear();
  initWasHereField(data);

  curPos.x = start.x;
  curPos.y = start.y;
  way.push_back(curPos);

  while (!isFinish(finish)) {
    checkNextStep(data, way);
    step(way);
  }
}