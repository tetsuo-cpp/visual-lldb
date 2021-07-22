#pragma once

#include "BreakpointModel.h"

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

namespace visual_lldb {

class LineNumberArea;

class CodeBrowser : public QPlainTextEdit {
  Q_OBJECT

public:
  CodeBrowser(QWidget *parent = nullptr);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();
  void contextMenuEvent(QContextMenuEvent *event) override;
  void updateHighlightedLines(const std::vector<Breakpoint> &bps,
                              size_t currentLine);

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void updateLineNumberArea(const QRect &rect, int dy);

private:
  QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget {
public:
  LineNumberArea(CodeBrowser *browser)
      : QWidget(browser), codeBrowser(browser) {}

  QSize sizeHint() const override {
    return QSize(codeBrowser->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) override {
    codeBrowser->lineNumberAreaPaintEvent(event);
  }

private:
  CodeBrowser *codeBrowser;
};

} // namespace visual_lldb
