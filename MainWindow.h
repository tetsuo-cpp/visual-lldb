#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace visual_lldb {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

private:
  Ui::MainWindow *ui;
};

} // namespace visual_lldb
