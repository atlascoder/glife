#include "lifemodel.h"
#include "life.h"

LifeModel::LifeModel(QObject* parent): QAbstractItemModel(parent),
    m_life(std::make_unique<GLife::Life>())
{

}

LifeModel::~LifeModel()
{

}

QModelIndex LifeModel::index(int row, int column, const QModelIndex&) const
{
    return this->createIndex(row, column);
}

QModelIndex LifeModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

int LifeModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(m_life->rows());
}

int LifeModel::columnCount(const QModelIndex&) const
{
    return static_cast<int>(m_life->cols());
}

QVariant LifeModel::data(const QModelIndex &index, int role) const
{
    if (isIndexValid(index)) {
        switch (role) {
        default:
            return QVariant(m_life->isAlive(static_cast<unsigned>(index.row()), static_cast<unsigned>(index.column())));
        }
    }
    else {
        return QVariant();
    }
}

bool LifeModel::isIndexValid(const QModelIndex &index) const
{
    return index.column() >= 0 && static_cast<unsigned>(index.column()) < m_life->cols()
           && index.row() >= 0 && static_cast<unsigned>(index.row()) < m_life->rows();
}

void LifeModel::toggle(const int row, const int col)
{
    if (row >=0 && row < static_cast<long>(m_life->rows()) && col >= 0 && static_cast<long>(m_life->cols())) {
        m_life->toggle(static_cast<unsigned>(row), static_cast<unsigned>(col));
        emit dataChanged(this->createIndex(row,col), this->createIndex(row,col));
    }
}

void LifeModel::setRows(const int value)
{
    if (value >= 0 && value != static_cast<long>(m_life->rows())) {
        m_life->setRows(static_cast<unsigned>(value));
        emit sizeChanged();
    }
}

void LifeModel::setCols(const int value)
{
    if (value >= 0 && value != static_cast<long>(m_life->rows())) {
        m_life->setCols(static_cast<unsigned>(value));
        emit sizeChanged();
    }
}

void LifeModel::cycle()
{
    m_life->cycle();
    emit dataChanged(this->index(0,0), this->index(m_life->rows(), m_life->cols()));
}
