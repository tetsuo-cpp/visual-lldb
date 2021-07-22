#include "CodeBrowser.h"

#include <QMenu>
#include <QPainter>
#include <QTextBlock>

namespace visual_lldb {

CodeBrowser::CodeBrowser(QWidget *parent) : QPlainTextEdit(parent) {
  lineNumberArea = new LineNumberArea(this);

  connect(this, &CodeBrowser::blockCountChanged, this,
          &CodeBrowser::updateLineNumberAreaWidth);
  connect(this, &CodeBrowser::updateRequest, this,
          &CodeBrowser::updateLineNumberArea);

  updateLineNumberAreaWidth(0);
}

int CodeBrowser::lineNumberAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

void CodeBrowser::contextMenuEvent(QContextMenuEvent *event) {
  QMenu *menu = createStandardContextMenu();
  menu->addAction(tr("Toggle Breakpoint"));
  menu->exec(event->globalPos());
  delete menu;
}

void CodeBrowser::updateLineNumberAreaWidth(int /* newBlockCount */) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeBrowser::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}

void CodeBrowser::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(
      QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeBrowser::updateHighlightedLines(const std::vector<Breakpoint> &bps,
                                         size_t currentLine) {
  QList<QTextEdit::ExtraSelection> extraSelections;
  // Mark each breakpoint as red.
  for (const auto &bp : bps) {
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor(document()->findBlockByLineNumber(bp.lineNumber - 1));
    QColor lineColor = QColor(Qt::red).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  // Mark current position as yellow.
  if (currentLine > 0) {
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor(document()->findBlockByLineNumber(currentLine));
    QColor lineColor = QColor(Qt::yellow).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void CodeBrowser::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top =
      qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

} // namespace visual_lldb
