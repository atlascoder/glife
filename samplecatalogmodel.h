#ifndef SAMPLECATALOGMODEL_H
#define SAMPLECATALOGMODEL_H

#include <QAbstractListModel>
#include <QBitmap>

#include "samplesprovider.h"

class SampleCatalogModel: public QAbstractListModel
{
    Q_OBJECT
    std::unique_ptr<SamplesProvider> mSamplesProvider;
public:
    SampleCatalogModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    enum Roles {
        FileRole = Qt::UserRole + 1,
        NameRole,
        WidthRole,
        HeightRole,
        BitmapRole
    };

    QHash<int, QByteArray> roleNames() const override;

};

#endif // SAMPLECATALOGMODEL_H
