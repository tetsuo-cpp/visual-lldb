#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include <lldb/API/SBDebugger.h>

namespace visual_lldb {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  // Call this before manipulating the window.
  ui->setupUi(this);

  // Create the debugger interface.
  QTextEdit *textEdit = new QTextEdit;
  textEdit->setReadOnly(true);
  QPushButton *bpButton = new QPushButton("Set breakpoint");
  QPushButton *runButton = new QPushButton("Run!");
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(textEdit);
  layout->addWidget(bpButton);
  layout->addWidget(runButton);
  QWidget *centralWidget = new QWidget;
  centralWidget->setLayout(layout);
  this->setCentralWidget(centralWidget);

  lldb::SBDebugger dbg;
}

MainWindow::~MainWindow() { delete ui; }

} // namespace visual_lldb
