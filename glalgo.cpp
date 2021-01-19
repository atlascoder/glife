#include "glalgo.h"

GLAlgo::GLAlgo():
    mGeneration(0),
    mPopulation(0),
    mLastGenerationTime(0),
    mWidth(defaultWidth),
    mHeight(defaultHeight),
    mSize(static_cast<size_t>(mWidth * mHeight)),
    mBorderRule(BorderRule::CLOSING)
{

}

GLAlgo::~GLAlgo()
{

}

void GLAlgo::setBorderRule(const BorderRule rule)
{
    if (rule == mBorderRule) return;
    mBorderRule = rule;
}
