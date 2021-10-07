#ifndef EXTRLEDIRECTORYSAMPLESPROVIDER_H
#define EXTRLEDIRECTORYSAMPLESPROVIDER_H

#include "samplesprovider.h"

class ExtRLEDirectorySamplesProvider: public SamplesProvider
{
public:
    ExtRLEDirectorySamplesProvider(const QString& directory);
};

#endif // EXTRLEDIRECTORYSAMPLESPROVIDER_H
