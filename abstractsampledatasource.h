#ifndef ABSTRACTSAMPLEDATASOURCE_H
#define ABSTRACTSAMPLEDATASOURCE_H

#include "sampledata.h"

class AbstractSampleDataSource
{
public:
    virtual ~AbstractSampleDataSource() {}

    virtual void reset() = 0;
    virtual SampleData next() = 0;
    virtual bool hasNext() = 0;
};

#endif // ABSTRACTSAMPLEDATASOURCE_H
