#include "mazegui.h"

#include "ui_mazegui.h"

MazeGui::MazeGui(QWidget *parent) : QMainWindow(parent), ui(new Ui::MazeGui) {
  ui->setupUi(this);
  this->move(300, 150);
  offset = 20;
  fieldSize = 500;
  clearEndPoints();
}

MazeGui::~MazeGui() { delete ui; }

void MazeGui::clearEndPoints() {
  startCoord.x = -1;
  startCoord.y = -1;
  finishCoord.x = -1;
  finishCoord.y = -1;
}

void MazeGui::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter p(this);
  p.setPen(QPen(Qt::gray, 3, Qt::SolidLine));
  p.setBrush(QColor(255, 255, 180));
  p.drawRect(offset - 2, offset - 2, fieldSize + 4, fieldSize + 4);
  if (ui->tabWidget->currentIndex() == 0) {
    if (data.mazeSize.size() && data.mazeSize[0] > 0 && data.mazeSize[1] > 0) {
      drawMaze(p);
      drawPoints(p);
      drawWay(p);
    }
  } else {
    if (caveData.caveSize.size() && caveData.caveSize[0] > 0 &&
        caveData.caveSize[1] > 0) {
      drawCave(p);
    }
  }
}

void MazeGui::drawMaze(QPainter &p) {
  double maxSide =
      data.mazeSize[0] > data.mazeSize[1] ? data.mazeSize[0] : data.mazeSize[1];
  cellSize = fieldSize / maxSide;

  p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
  p.drawRect(offset, offset, cellSize * data.mazeSize[1],
             cellSize * data.mazeSize[0]);
  p.setPen(QPen(Qt::black, 2, Qt::SolidLine));

  int i = 0;
  for (auto rows : data.rightWalls) {
    for (auto j = 0U; j < rows.size(); ++j) {
      if (data.rightWalls[i][j]) {
        p.drawLine(offset + cellSize + j * cellSize, offset + i * cellSize,
                   offset + cellSize + j * cellSize,
                   offset + cellSize + i * cellSize);
      }
      if (data.bottomWalls[i][j]) {
        p.drawLine(offset + j * cellSize, offset + cellSize + i * cellSize,
                   offset + cellSize + j * cellSize,
                   offset + cellSize + i * cellSize);
      }
    }
    ++i;
  }
}

void MazeGui::drawCave(QPainter &p) {
  double maxSide = caveData.caveSize[0] > caveData.caveSize[1]
                       ? caveData.caveSize[0]
                       : caveData.caveSize[1];
  double cellCaveSize = fieldSize / maxSide;

  p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
  p.drawRect(offset, offset, cellCaveSize * caveData.caveSize[1],
             cellCaveSize * caveData.caveSize[0]);
  p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
  p.setBrush(QColor(10, 10, 10));

  int i = 0;
  for (auto rows : caveData.field) {
    for (auto j = 0U; j < rows.size(); ++j) {
      if (caveData.field[i][j]) {
        p.drawRect(offset + j * cellCaveSize, offset + i * cellCaveSize,
                   cellCaveSize, cellCaveSize);
      }
    }
    ++i;
  }
}

void MazeGui::drawPoints(QPainter &p) {
  p.setPen(QPen(Qt::transparent, 1, Qt::SolidLine));

  if (startCoord.x >= 0 && startCoord.y >= 0) {
    p.setBrush(QColor(130, 250, 120));
    p.drawRect(offset + startCoord.x + (cellSize * 0.2),
               offset + startCoord.y + (cellSize * 0.2), cellSize * 0.6,
               cellSize * 0.6);
  }
  if (finishCoord.x >= 0 && finishCoord.y >= 0) {
    p.setBrush(QColor(170, 220, 250));
    p.drawRect(offset + finishCoord.x + (cellSize * 0.2),
               offset + finishCoord.y + (cellSize * 0.2), cellSize * 0.6,
               cellSize * 0.6);
  }
}

void MazeGui::drawWay(QPainter &p) {
  p.setPen(QPen(Qt::red, 2, Qt::SolidLine));
  if (way.size() > 1) {
    for (size_t i = 0; i < way.size() - 1; ++i) {
      p.drawLine(offset + way[i].x * cellSize + cellSize / 2,
                 offset + way[i].y * cellSize + cellSize / 2,
                 offset + way[i + 1].x * cellSize + cellSize / 2,
                 offset + way[i + 1].y * cellSize + cellSize / 2);
    }
  }
}

void MazeGui::on_loadBtn_clicked() {
  int error = 0;
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*.*)"));
  try {
    C.clearMazeData(data, way);
    C.loadFile(data, filePath.toStdString());
  } catch (...) {
    error = 1;
  }
  if (error) {
    ui->statusLine->setStyleSheet("color:red");
    ui->statusLine->setText("Ошибка чтения файла!");
  } else {
    update();
  }
}

void MazeGui::on_saveBtn_clicked() {
  std::string name = "maze.txt";
  QString fileName = QFileDialog::getSaveFileName(this, tr("Open Address book"),
                                                  QString::fromStdString(name),
                                                  tr("Address book (*.txt)"));
  C.saveFile(data, fileName.toStdString());
  ui->statusLine->setStyleSheet("color:green");
  ui->statusLine->setText("Лабиринт успешно сохранён!");
}

void MazeGui::on_genBtn_clicked() {
  C.clearMazeData(data, way);
  C.generate(data, ui->rowsSpin->text().toInt(), ui->colsSpin->text().toInt());
  if (data.mazeSize[0] && data.mazeSize[1]) {
    update();
    clearEndPoints();
    isGenerated = true;
  }
}

void MazeGui::mousePressEvent(QMouseEvent *event) {
  int x = event->x() - offset;
  int y = event->y() - offset;
  if (x >= 0 && x <= fieldSize && y >= 0 && y <= fieldSize) {
    if (isStartPressed && ui->tabWidget->currentIndex() == 0) {
      startCoord.x = floor(x / cellSize) * cellSize;
      startCoord.y = floor(y / cellSize) * cellSize;
      startCoordforModel.x = round(startCoord.x / cellSize);
      startCoordforModel.y = round(startCoord.y / cellSize);
      isStartPressed = false;
    }
    if (isFinishPressed && ui->tabWidget->currentIndex() == 0) {
      finishCoord.x = floor(x / cellSize) * cellSize;
      finishCoord.y = floor(y / cellSize) * cellSize;
      finishCoordforModel.x = round(finishCoord.x / cellSize);
      finishCoordforModel.y = round(finishCoord.y / cellSize);
      isFinishPressed = false;
    }
    ui->loadBtn->setEnabled(true);
    ui->saveBtn->setEnabled(true);
    ui->genBtn->setEnabled(true);
    ui->rowsSpin->setEnabled(true);
    ui->colsSpin->setEnabled(true);
    ui->startBtn->setEnabled(true);
    ui->endBtn->setEnabled(true);
    ui->findBtn->setEnabled(true);
    ui->labelRows->setEnabled(true);
    ui->labelCols->setEnabled(true);
    update();
    ui->statusLine->setText("");
  }
}

void MazeGui::on_findBtn_clicked() {
  if (startCoord.x >= 0 && startCoord.y >= 0 && finishCoord.x >= 0 &&
      finishCoord.y >= 0) {
    C.findWay(data, way, startCoordforModel, finishCoordforModel);
    update();
  } else {
    ui->statusLine->setStyleSheet("color:red");
    ui->statusLine->setText("Необходимо указать точки маршрута");
  }
}

void MazeGui::disableUi() {
  ui->loadBtn->setEnabled(false);
  ui->saveBtn->setEnabled(false);
  ui->genBtn->setEnabled(false);
  ui->rowsSpin->setEnabled(false);
  ui->colsSpin->setEnabled(false);
  ui->startBtn->setEnabled(false);
  ui->endBtn->setEnabled(false);
  ui->findBtn->setEnabled(false);
  ui->labelRows->setEnabled(false);
  ui->labelCols->setEnabled(false);
  ui->statusLine->setStyleSheet("color:blue");
}

void MazeGui::on_startBtn_clicked() {
  if (isGenerated) {
    isStartPressed = true;
    disableUi();
    ui->statusLine->setText("Укажите начальную точку маршрута");
  }
}

void MazeGui::on_endBtn_clicked() {
  if (isGenerated) {
    isFinishPressed = true;
    disableUi();
    ui->statusLine->setText("Укажите конечную точку маршрута");
    ;
  }
}

void MazeGui::on_caveGenBtn_clicked() {
  caveData.limitBorn = ui->bornSpin->text().toInt();
  caveData.limitDie = ui->deathSpin->text().toInt();
  caveData.startChance = ui->chanceSpin->text().toInt();
  C.clearCaveData(caveData);
  C.generateCave(caveData, ui->rowsSpin_2->text().toInt(),
                 ui->colsSpin_2->text().toInt());
  if (caveData.caveSize[0] && caveData.caveSize[1]) {
    update();
    enableCaveUi();
    isCaveGenerated = false;
  }
}

void MazeGui::on_tabWidget_currentChanged(int index) {
  Q_UNUSED(index);
  update();
  isStartPressed = false;
  isFinishPressed = false;
  ui->statusLine->setText("");
}

void MazeGui::on_nextStepBtn_clicked() {
  isCaveGenerated = C.formalization(caveData);
  isStopped = false;
  if (!isCaveGenerated) {
    ui->nextStepBtn->setEnabled(false);
    ui->autoIntervalBtn->setEnabled(false);
    ui->stepSpin->setEnabled(false);
    ui->labelStep->setEnabled(false);
    ui->statusLine->setStyleSheet("color:green");
    ui->statusLine->setText("Генерация пещеры завершена");
  } else {
    update();
  }
}

void MazeGui::on_autoIntervalBtn_clicked() {
  int timer = 0;
  isStopped = false;
  ui->nextStepBtn->setEnabled(false);
  ui->autoIntervalBtn->setEnabled(false);
  ui->stepSpin->setEnabled(false);
  ui->labelStep->setEnabled(false);
  ui->saveCaveBtn->setEnabled(false);
  ui->loadCaveBtn->setEnabled(false);
  ui->caveGenBtn->setEnabled(false);

  isCaveGenerated = C.formalization(caveData);
  while (isCaveGenerated) {
    ui->stopBtn->setEnabled(true);
    QCoreApplication::processEvents();
    isCaveGenerated = C.formalization(caveData);
    update();
    timer += ui->stepSpin->text().toInt();
    QThread::msleep(ui->stepSpin->text().toInt());
    if (timer > 30000 || isStopped) break;
  }
  ui->stopBtn->setEnabled(false);
  if (timer > 20000) {
    enableCaveUi();
    ui->saveCaveBtn->setEnabled(true);
    ui->loadCaveBtn->setEnabled(true);
    ui->caveGenBtn->setEnabled(true);
    ui->statusLine->setStyleSheet("color:red");
    ui->statusLine->setText("Превышено время ожидания");
    isCaveGenerated = false;
  }

  if (!isCaveGenerated) {
    ui->saveCaveBtn->setEnabled(true);
    ui->loadCaveBtn->setEnabled(true);
    ui->caveGenBtn->setEnabled(true);
    ui->statusLine->setStyleSheet("color:green");
    ui->statusLine->setText("Генерация пещеры завершена");
  }
}

void MazeGui::on_loadCaveBtn_clicked() {
  int error = 0;
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*.*)"));
  try {
    C.clearCaveData(caveData);
    C.loadCaveFile(caveData, filePath.toStdString());
  } catch (...) {
    error = 1;
  }
  if (error) {
    ui->statusLine->setStyleSheet("color:red");
    ui->statusLine->setText("Ошибка чтения файла!");
  } else {
    update();
    enableCaveUi();
  }
}

void MazeGui::enableCaveUi() {
  ui->statusLine->setText("");
  ui->nextStepBtn->setEnabled(true);
  ui->autoIntervalBtn->setEnabled(true);
  ui->stepSpin->setEnabled(true);
  ui->labelStep->setEnabled(true);
}

void MazeGui::on_saveCaveBtn_clicked() {
  std::string name = "cave.txt";
  QString fileName = QFileDialog::getSaveFileName(this, tr("Open Address book"),
                                                  QString::fromStdString(name),
                                                  tr("Address book (*.txt)"));
  C.saveCaveFile(caveData, fileName.toStdString());
  ui->statusLine->setStyleSheet("color:green");
  ui->statusLine->setText("Пещера успешно сохранёна!");
}

void MazeGui::on_stopBtn_clicked() {
  isStopped = true;
  enableCaveUi();
  ui->saveCaveBtn->setEnabled(true);
  ui->loadCaveBtn->setEnabled(true);
  ui->caveGenBtn->setEnabled(true);
  ui->statusLine->setStyleSheet("color:red");
  ui->statusLine->setText("Остановлено пользователем");
}
