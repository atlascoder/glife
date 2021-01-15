#ifndef SAMPLESCATALOG_H
#define SAMPLESCATALOG_H

#include <QAbstractListModel>
#include <QBitmap>

struct RleSample {
    QString mFile;
    QString mName;
    QBitmap mBitmap;
    int mWidth;
    int mHeight;
};

class SamplesCatalog: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString folderPath READ folderPath WRITE setFolderPath NOTIFY folderPathChanged)
public:
    SamplesCatalog(QObject* parent = nullptr);

    QString folderPath() const { return mFolderPath; }
    void setFolderPath(const QString& value);

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

signals:
    void folderPathChanged();

protected:
    void loadSamples();

private:
    QString mFolderPath;
    QVector<RleSample> mSamples;
};

#endif // SAMPLESCATALOG_H
