#ifndef GLALGO_H
#define GLALGO_H

#include "borderrule.h"

class QBitmap;
class QString;

typedef long long unsigned int size_t;

class GLAlgo
{
public:
    static const int defaultWidth = 400;
    static const int defaultHeight = 400;
    GLAlgo();
    virtual ~GLAlgo();
    virtual QBitmap drawUniverse() const = 0;
    virtual QString name() const = 0;
    virtual void setSize(unsigned width, unsigned height) = 0;
    virtual void reset() = 0;
    virtual void nextGen() = 0;
    virtual void insertSample(const QBitmap& other, unsigned x, unsigned y) = 0;
    virtual void setUniverse(const QBitmap& universe)=0;
    virtual bool isAlive(unsigned x, unsigned y) const = 0;
    virtual void toggle(unsigned x, unsigned y) = 0;
    virtual void randomize() = 0;


    size_t generation() const { return mGeneration; }
    size_t population() const { return mPopulation; }
    unsigned long lastGenerationTime() const { return mLastGenerationTime; }
    unsigned width() const { return mWidth; }
    unsigned height() const { return mHeight; }
    BorderRule borderRule() const { return mBorderRule; }
    void setBorderRule(const BorderRule rule);

protected:
    size_t mGeneration;
    size_t mPopulation;
    unsigned long mLastGenerationTime;
    unsigned mWidth;
    unsigned mHeight;
    size_t mSize;
    BorderRule mBorderRule;
};

#endif // GLALGO_H
