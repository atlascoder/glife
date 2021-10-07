#include "directorysampledatasource.h"

#include <QDir>
#include <QDebug>

DirectorySampleDataSource::DirectorySampleDataSource(const QString& dir, const QString& ext):
    AbstractSampleDataSource(),
    mDirectory(dir.isEmpty() ? "rle" : dir),
    mFileExtention(ext.isEmpty() ? "*" : ext)
{

}

void DirectorySampleDataSource::reset()
{
    qDebug() << mDirectory << " / " << mFileExtention;
    mFiles = QDir(mDirectory).entryList(QStringList() << QString("*.").append(mFileExtention), QDir::Files);
    mIterator = mFiles.begin();
}

SampleData DirectorySampleDataSource::next()
{
    SampleData sd;
    if (mIterator != mFiles.end()) {
        QString filename(*mIterator);
        QFile file(mDirectory + "/" + filename);
        sd.id = filename;
        if (file.open(QFile::ReadOnly)) {
            sd.append(file.readAll());
            file.close();
        }
        mIterator++;
    }
    return sd;
}

bool DirectorySampleDataSource::hasNext()
{
    return mIterator != mFiles.end();
}
