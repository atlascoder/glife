#ifndef LIFEMODEL_H
#define LIFEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

namespace GLife {
    class Life;
}

class LifeModel: public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rowCount WRITE setRows NOTIFY sizeChanged)
    Q_PROPERTY(int cols READ columnCount WRITE setCols NOTIFY sizeChanged)
public:
    LifeModel(QObject* parent = nullptr);
    ~LifeModel() override;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void toggle(const int row, const int col);
    Q_INVOKABLE void cycle();

    bool isIndexValid(const QModelIndex& index) const;

    void setRows(const int value);
    void setCols(const int value);

signals:
    void sizeChanged();
private:
    std::unique_ptr<GLife::Life> m_life;
};

#endif // LIFEMODEL_H
