#include "glnaive.h"

#include <QBitmap>
#include <QImage>
#include <QString>
#include <random>
#include <chrono>

#include "universe.h"

GLNaive::GLNaive(): GLAlgo(),
    mUniverse(new Universe(mHeight, mWidth))
{
    mUniverse->clear();
}

GLNaive::~GLNaive()
{
    delete mUniverse;
}

QString GLNaive::name() const
{
    return "Naive";
}

void GLNaive::setSize(unsigned width, unsigned height)
{
    mWidth = width;
    mHeight = height;
    mUniverse->resize(mHeight, mWidth);
    mUniverse->clear();
    mSize = mUniverse->lengthInBits();
}

void GLNaive::reset()
{
    mUniverse->clear();
    mGeneration = 0;
}

void GLNaive::nextGen()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    Universe& bm = *mUniverse;
    Universe* next_mat(new Universe(mHeight, mWidth));
    Universe& next = *next_mat;
    next.clear();
    mPopulation = 0;
    char (Universe::*countFunc)(unsigned, unsigned);
    switch (mBorderRule) {
    case BorderRule::CLOSING:
        countFunc = &Universe::nsWithClosingB;
        break;
    case BorderRule::ALIVE:
        countFunc = &Universe::nsWithAliveB;
        break;
    case BorderRule::DEAD:
        countFunc = &Universe::nsWithDeadB;
        break;
    default:
        countFunc = &Universe::nsWithClosingB;
    }

    unsigned rows = mUniverse->rows();
    unsigned cols = mUniverse->cols();
    unsigned i = 0;

    for (unsigned row = 0; row < rows; row++) {
        for (unsigned col = 0; col < cols; col++) {
            char count = (mUniverse->*countFunc)(row, col);
            if (count == 3 || (count == 2 && bm[i])) {
                next.set(i, true);
                mPopulation++;
            }
            i++;
        }
    }

    std::swap(mUniverse, next_mat);
    delete next_mat;
    mGeneration++;
    auto t2 = std::chrono::high_resolution_clock::now();
    mLastGenerationTime = static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
}

void GLNaive::insertSample(const QBitmap &other, unsigned x, unsigned y)
{
    Universe& bm = *mUniverse;
    unsigned right = x + static_cast<unsigned>(other.width()) > bm.cols() ? bm.cols() : x + static_cast<unsigned>(other.width());
    unsigned bottom = y + static_cast<unsigned>(other.height()) > bm.rows() ? bm.rows() : y + static_cast<unsigned>(other.height());
    QImage img = other.toImage();
    for (unsigned row = y; row < bottom; row++) {
        for (unsigned col = x; col < right; col++) {
            if (img.pixelColor(static_cast<int>(col-x), static_cast<int>(row-y)).black()) bm.set(row, col, true);
        }
    }
}

void GLNaive::setUniverse(const QBitmap&)
{

}

QBitmap GLNaive::drawUniverse() const
{
    return QBitmap::fromData(QSize(static_cast<int>(mWidth), static_cast<int>(mHeight)), mUniverse->data(), QImage::Format::Format_Mono);
}

bool GLNaive::isAlive(unsigned x, unsigned y) const
{
    unsigned long idx = x + y * mWidth;
    return (*mUniverse)[idx];
}

void GLNaive::toggle(unsigned x, unsigned y)
{
    unsigned long idx = x + y * mWidth;
    mUniverse->set(idx, !(*mUniverse)[idx]);
}



void GLNaive::randomize()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> rg;
    size_t len = mUniverse->lengthInBits() / sizeof (int) / 8;
    unsigned* p_mat_ints = reinterpret_cast<unsigned*>(mUniverse->data());
    mPopulation = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned val = rg(gen);
        for (unsigned i = 0; i < sizeof(unsigned)*8; i++)
            if (val & (1 < i)) mPopulation++;
        p_mat_ints[i] = val;
    }
    mGeneration = 0;
}
