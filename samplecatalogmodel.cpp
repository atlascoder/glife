#include "samplecatalogmodel.h"

#include "extrledirectorysamplesprovider.h"

SampleCatalogModel::SampleCatalogModel(QObject* parent):
    QAbstractListModel(parent),
    mSamplesProvider(std::move(new ExtRLEDirectorySamplesProvider("rle")))
{

}

int SampleCatalogModel::rowCount(const QModelIndex &parent) const
{
    return mSamplesProvider->sampleCount();
}

QHash<int, QByteArray> SampleCatalogModel::roleNames() const
{
    return QHash<int, QByteArray>{
            { Roles::FileRole,      "file" },
            { Roles::NameRole,      "name" },
            { Roles::WidthRole,     "width" },
            { Roles::HeightRole,    "height" },
            { Roles::BitmapRole,    "bitmap" }
        };
}

QVariant SampleCatalogModel::data(const QModelIndex &index, int role) const
{
    const auto& samples = *mSamplesProvider;

    if (index.row() < 0 || index.row() >= samples.sampleCount()) return QVariant();

    switch (role) {
    case Roles::FileRole:
        return samples[index.row()].id();
    case Roles::NameRole:
        return samples[index.row()].name();
    case Roles::BitmapRole:
        return samples[index.row()].bitmap();
    case Roles::WidthRole:
        return samples[index.row()].width();
    case Roles::HeightRole:
        return samples[index.row()].height();
    default:
        return QVariant();
    }
}
