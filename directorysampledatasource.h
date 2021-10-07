#ifndef DIRECTORYSAMPLEDATASOURCE_H
#define DIRECTORYSAMPLEDATASOURCE_H

#include "abstractsampledatasource.h"
#include <QString>
#include <QStringList>

class DirectorySampleDataSource: public AbstractSampleDataSource
{
    QString mDirectory;
    QString mFileExtention;
    QStringList mFiles;
    QStringList::iterator mIterator;
public:
    DirectorySampleDataSource(const QString& directory, const QString& fileExtension);

    void reset() override;
    SampleData next() override;
    bool hasNext() override;
};

#endif // DIRECTORYSAMPLEDATASOURCE_H
