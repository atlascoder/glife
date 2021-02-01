#ifndef GLNAIVE_H
#define GLNAIVE_H

#include "glalgo.h"

#include <QString>
#include <memory>

class Universe;

class GLNaive: public GLAlgo
{
public:
    GLNaive();
    virtual ~GLNaive() override;

    QBitmap drawUniverse() const override;
    QString name() const override;
    void setSize(unsigned width, unsigned height) override;
    void reset() override;
    void nextGen() override;
    void insertSample(const QBitmap& other, unsigned x, unsigned y) override;
    void setUniverse(const QBitmap& universe) override;
    bool isAlive(unsigned x, unsigned y) const override;
    void toggle(unsigned x, unsigned y) override;
    void randomize() override;
protected:
    Universe* mUniverse;
};

#endif // GLNAIVE_H
