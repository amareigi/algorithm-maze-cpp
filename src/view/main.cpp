#include <QApplication>

#include "mazegui.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MazeGui w;
  w.show();
  return a.exec();
}
