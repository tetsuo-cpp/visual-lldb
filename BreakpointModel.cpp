#include "BreakpointModel.h"

namespace visual_lldb {

BreakpointModel::BreakpointModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int BreakpointModel::rowCount(const QModelIndex &) const { return bps.size(); }

int BreakpointModel::columnCount(const QModelIndex &) const {
  // File, Line
  return 2;
}

QVariant BreakpointModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    assert(index.row() < bps.size());
    const auto &bp = bps.at(index.row());
    switch (index.column()) {
    case 0:
      return QString(bp.filePath.c_str());
    case 1:
      return bp.lineNumber;
    default:
      assert(false);
      break;
    }
  }
  return QVariant();
}

void BreakpointModel::setBreakpoints(const std::vector<Breakpoint> &newBps) {
  const size_t prevSize = bps.size();
  bps = newBps;
  int diff = bps.size() - prevSize;
  if (diff > 0)
    insertRows(prevSize, diff);
  else if (diff < 0)
    removeRows(bps.size(), diff);
  emit layoutChanged();
  // Emit changes.
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(bps.size() - 1, 1);
  emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

} // namespace visual_lldb
