#include "BreakpointModel.h"

namespace visual_lldb {

BreakpointModel::BreakpointModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int BreakpointModel::rowCount(const QModelIndex &) const { return bps.size(); }

int BreakpointModel::columnCount(const QModelIndex &) const {
  // ID, File, Line
  return 3;
}

QVariant BreakpointModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    assert(index.row() < bps.size());
    const auto &bp = bps.at(index.row());
    switch (index.column()) {
    case 0:
      return index.column();
    case 1:
      return QString(bp.filePath.c_str());
    case 2:
      return bp.lineNumber;
    default:
      assert(false);
      break;
    }
  }
  return QVariant();
}

void BreakpointModel::setBreakpoints(std::vector<Breakpoint> &&newBps) {
  const size_t prevSize = bps.size();
  bps = std::move(newBps);
  if (bps.empty())
    return;
  beginInsertRows(QModelIndex(), prevSize, bps.size() - prevSize);
  insertRows(rowCount(), bps.size() - prevSize);
  endInsertRows();
  // Emit changes.
  QModelIndex topLeft = index(0, 0);
  QModelIndex bottomRight = index(bps.size() - 1, 2);
  emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
}

} // namespace visual_lldb
