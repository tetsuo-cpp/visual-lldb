#include "FrameVariableModel.h"

namespace visual_lldb {

FrameVariableModel::FrameVariableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int FrameVariableModel::rowCount(const QModelIndex &) const {
  return frameVariables.size();
}

int FrameVariableModel::columnCount(const QModelIndex &) const {
  // Name, Value
  return 2;
}

QVariant FrameVariableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    assert(index.row() < frameVariables.size());
    const auto &fv = frameVariables.at(index.row());
    switch (index.column()) {
    case 0:
      return QString(fv.name.c_str());
    case 1:
      return QString(fv.value.c_str());
    default:
      assert(false);
      break;
    }
  }
  return QVariant();
}

void FrameVariableModel::setFrameVariables(
    std::vector<FrameVariable> &&newFrameVariables) {
  const size_t prevSize = frameVariables.size();
  frameVariables = std::move(newFrameVariables);
  int diff = frameVariables.size() - prevSize;
  if (diff > 0)
    insertRows(prevSize, diff);
  else if (diff < 0)
    removeRows(frameVariables.size(), diff);
  emit layoutChanged();
  // Emit changes.
  QModelIndex topLeft = createIndex(0, 0);
  QModelIndex bottomRight = createIndex(frameVariables.size() - 1, 1);
  emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

} // namespace visual_lldb
