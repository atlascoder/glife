#include "samplesprovider.h"

#include "abstractsampleconverter.h"
#include "abstractsampledatasource.h"
#include <QDebug>

SamplesProvider::SamplesProvider(std::unique_ptr<AbstractSampleDataSource> dataSource, std::unique_ptr<AbstractSampleConverter> sampleConverter):
    mDataSource(std::move(dataSource)),
    mSampleConverter(std::move(sampleConverter)),
    mSamples()

{
    reload();
}

void SamplesProvider::reload()
{
    mSamples.clear();
    mDataSource->reset();
    while(mDataSource->hasNext()) {
        Sample sd = mSampleConverter->fromSampleData(mDataSource->next());
        if (mSampleConverter->error().isEmpty()) {
            mSamples.append(std::move(sd));
        }
    }
}

int SamplesProvider::sampleCount() const
{
    return mSamples.length();
}

Sample SamplesProvider::operator[](int index) const
{
    return mSamples[index];
}
