#pragma once

#include <QAbstractTableModel>

namespace visual_lldb {

struct FrameVariable {
  std::string name, value;
};

class FrameVariableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  FrameVariableModel(QObject *parent = nullptr);
  virtual ~FrameVariableModel() = default;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  void setFrameVariables(std::vector<FrameVariable> &&frameVariables);

private:
  std::vector<FrameVariable> frameVariables;
};

} // namespace visual_lldb
