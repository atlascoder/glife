#include "samplescatalog.h"

#include <QDir>
#include <QStringBuilder>

#include "rlereader.h"

SamplesCatalog::SamplesCatalog(QObject* parent):
    QAbstractListModel(parent),
    mFolderPath("rle"),
    mSamples()
{
    loadSamples();
}

void SamplesCatalog::setFolderPath(const QString &value)
{
    if (value == mFolderPath) return;
    mFolderPath = value;
    emit folderPathChanged();
}

void SamplesCatalog::loadSamples()
{
    beginResetModel();
    mSamples.clear();
    QString folder_path = mFolderPath.isNull() ? "rle" : mFolderPath;
    QStringList files = QDir(folderPath()).entryList(QStringList() << "*.rle", QDir::Files);
    for (auto& file: files) {
        RleReader reader(folder_path % "/" % file);
        if (reader.isValid()) {
            mSamples.append({file, reader.name(), reader.bitmap(), reader.width(), reader.height()});
        }
    }
    endResetModel();
}

QHash<int, QByteArray> SamplesCatalog::roleNames() const
{
    return QHash<int, QByteArray>{
            { Roles::FileRole,      "file" },
            { Roles::NameRole,      "name" },
            { Roles::WidthRole,     "width" },
            { Roles::HeightRole,    "height" },
            { Roles::BitmapRole,    "bitmap" }
        };
}

QVariant SamplesCatalog::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mSamples.size()) return QVariant();

    switch (role) {
    case Roles::FileRole:
        return mSamples[index.row()].mFile;
    case Roles::NameRole:
        return mSamples[index.row()].mName;
    case Roles::BitmapRole:
        return mSamples[index.row()].mBitmap;
    case Roles::WidthRole:
        return mSamples[index.row()].mWidth;
    case Roles::HeightRole:
        return mSamples[index.row()].mHeight;
    default:
        return QVariant();
    }
}

int SamplesCatalog::rowCount(const QModelIndex &) const
{
    return mSamples.size();
}
