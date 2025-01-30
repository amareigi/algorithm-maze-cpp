#include <gtest/gtest.h>

#include "../common.h"
#include "../controller/controller.hpp"

TEST(Maze_Cave, read_maze_file) {
  std::string filenameMaze = "files/example_maze.txt";
  Controller c;
  Data data;
  EXPECT_NO_THROW(c.loadFile(data, filenameMaze));
}

TEST(Maze_Cave, read_cave_file) {
  std::string filenameCave = "files/example_cave.txt";
  Controller c;
  CaveData caveData;
  caveData.startChance = 50;
  EXPECT_NO_THROW(c.loadCaveFile(caveData, filenameCave));
}

TEST(Maze_Cave, icorrect_maze_file) {
  std::string filenameMaze = "files/example_maze1.txt";
  Controller c;
  Data data;
  EXPECT_ANY_THROW(c.loadFile(data, filenameMaze));
}

TEST(Maze_Cave, icorrect_cave_file) {
  std::string filenameCave = "files/example_cave1.txt";
  Controller c;
  CaveData caveData;
  caveData.startChance = 50;
  EXPECT_ANY_THROW(c.loadCaveFile(caveData, filenameCave));
}

TEST(Maze_Cave, save_maze_file) {
  std::string filenameMaze = "files/example_maze.txt";
  Controller c;
  Data data;
  c.loadFile(data, filenameMaze);
  EXPECT_NO_THROW(c.saveFile(data, "test_maze.txt"));
}

TEST(Maze_Cave, save_cave_file) {
  std::string filenameCave = "files/example_cave.txt";
  Controller c;
  CaveData caveData;
  caveData.startChance = 50;
  c.loadCaveFile(caveData, filenameCave);
  EXPECT_NO_THROW(c.saveCaveFile(caveData, "test_cave.txt"));
}

TEST(Maze_Cave, maze_generate) {
  Controller c;
  Data data;
  int rows = 25;
  int cols = 25;
  EXPECT_NO_THROW(c.generate(data, rows, cols));
  EXPECT_NO_THROW(c.generate(data, rows, cols));
  EXPECT_NO_THROW(c.generate(data, rows, cols));
  EXPECT_NO_THROW(c.generate(data, rows, cols));
}

TEST(Maze_Cave, cave_generate) {
  Controller c;
  CaveData caveData;
  caveData.startChance = 50;
  int rows = 25;
  int cols = 25;
  EXPECT_NO_THROW(c.generateCave(caveData, rows, cols));
  EXPECT_NO_THROW(c.generateCave(caveData, rows, cols));
  EXPECT_NO_THROW(c.generateCave(caveData, rows, cols));
  EXPECT_NO_THROW(c.generateCave(caveData, rows, cols));
}

TEST(Maze_Cave, maze_find_way) {
  Controller c;
  Data data;
  way_t way;
  c.clearMazeData(data, way);
  Coord_t start = {0, 0};
  Coord_t finish = {24, 24};
  int rows = 25;
  int cols = 25;
  c.generate(data, rows, cols);
  c.findWay(data, way, start, finish);
  EXPECT_EQ(0, way[0].x);
  EXPECT_EQ(0, way[0].y);
  EXPECT_EQ(24, way.back().x);
  EXPECT_EQ(24, way.back().y);
}

TEST(Maze_Cave, cave_formalization) {
  Controller c;
  CaveData caveData;
  c.clearCaveData(caveData);
  caveData.startChance = 50;
  caveData.limitBorn = 4;
  caveData.limitDie = 4;
  int rows = 25;
  int cols = 25;

  c.generateCave(caveData, rows, cols);
  EXPECT_TRUE(c.formalization(caveData));
  for (int i = 0; i < 50; ++i) c.formalization(caveData);
  EXPECT_FALSE(c.formalization(caveData));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
