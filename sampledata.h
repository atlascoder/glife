#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H

#include <QByteArray>
#include <QString>

class SampleData: public QByteArray
{
public:
    QString id;
};

#endif // SAMPLEDATA_H
