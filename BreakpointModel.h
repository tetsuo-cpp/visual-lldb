#pragma once

#include <QAbstractTableModel>

namespace visual_lldb {

struct Breakpoint {
  std::string filePath;
  int lineNumber;
};

class BreakpointModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit BreakpointModel(QObject *parent = nullptr);
  virtual ~BreakpointModel() = default;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  void setBreakpoints(const std::vector<Breakpoint> &newBps);

private:
  std::vector<Breakpoint> bps;
};

} // namespace visual_lldb
