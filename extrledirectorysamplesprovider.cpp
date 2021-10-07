#include "extrledirectorysamplesprovider.h"

#include "directorysampledatasource.h"
#include "extrlesampleconverter.h"

ExtRLEDirectorySamplesProvider::ExtRLEDirectorySamplesProvider(const QString& directory):
    SamplesProvider(std::unique_ptr<DirectorySampleDataSource>(new DirectorySampleDataSource(directory, "rle")),
                    std::unique_ptr<ExtRLESampleConverter>(new ExtRLESampleConverter))
{

}
