#ifndef SAMPLESPROVIDER_H
#define SAMPLESPROVIDER_H

#include <QVector>
#include <QList>
#include <memory>

#include "sample.h"
#include "abstractsampledatasource.h"
#include "abstractsampleconverter.h"

class SamplesProvider
{
    std::unique_ptr<AbstractSampleDataSource> mDataSource;
    std::unique_ptr<AbstractSampleConverter> mSampleConverter;
    QList<Sample> mSamples;
public:
    SamplesProvider(std::unique_ptr<AbstractSampleDataSource> dataSource, std::unique_ptr<AbstractSampleConverter> sampleConverter);
    virtual ~SamplesProvider() {}

    virtual void reload();

    int sampleCount() const;
    Sample operator[](int index) const;

//    const QList<Sample>& samples() const;
};

#endif // SAMPLESPROVIDER_H
