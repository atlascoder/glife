#ifndef EXTRLESAMPLECONVERTER_H
#define EXTRLESAMPLECONVERTER_H

#include "abstractsampleconverter.h"

class ExtRLESampleConverter: public AbstractSampleConverter
{
public:
    Sample fromSampleData(const SampleData& data) override;
    SampleData toSampleData(const Sample& sample) override;
};

#endif // EXTRLESAMPLECONVERTER_H
