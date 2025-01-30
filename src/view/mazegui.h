#ifndef MAZEGUI_H
#define MAZEGUI_H

#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QThread>

#include "../controller/controller.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MazeGui;
}
QT_END_NAMESPACE

class MazeGui : public QMainWindow {
  Q_OBJECT

 public:
  MazeGui(QWidget *parent = nullptr);
  ~MazeGui();

  void drawMaze(QPainter &p);
  void drawPoints(QPainter &p);
  void drawWay(QPainter &p);
  void drawCave(QPainter &p);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

 private slots:
  void on_loadBtn_clicked();
  void on_saveBtn_clicked();
  void on_genBtn_clicked();
  void on_findBtn_clicked();
  void on_endBtn_clicked();
  void on_startBtn_clicked();
  void on_caveGenBtn_clicked();
  void on_tabWidget_currentChanged(int index);
  void on_nextStepBtn_clicked();
  void on_autoIntervalBtn_clicked();
  void on_loadCaveBtn_clicked();
  void on_saveCaveBtn_clicked();
  void on_stopBtn_clicked();

 private:
  Ui::MazeGui *ui;
  Data data;
  way_t way;
  CaveData caveData;
  Controller C;
  int offset;
  int fieldSize;
  double cellSize;
  bool isStartPressed = false;
  bool isFinishPressed = false;
  Coord_t startCoord;
  Coord_t finishCoord;
  Coord_t startCoordforModel;
  Coord_t finishCoordforModel;
  bool isGenerated = false;
  bool isCaveGenerated = false;
  bool isStopped = false;

  void clearEndPoints();
  void disableUi();
  void enableCaveUi();
};

#endif  // MAZEGUI_H
