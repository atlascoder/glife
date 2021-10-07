#ifndef ABSTRACTSAMPLECONVERTER_H
#define ABSTRACTSAMPLECONVERTER_H

#include <QString>

#include "sample.h"
#include "sampledata.h"

class AbstractSampleConverter
{
protected:
    QString mError;
public:
    virtual ~AbstractSampleConverter() {}

    virtual Sample fromSampleData(const SampleData& data) = 0;
    virtual SampleData toSampleData(const Sample& sample) = 0;

    QString error() const { return mError; }
};

#endif // ABSTRACTSAMPLECONVERTER_H
