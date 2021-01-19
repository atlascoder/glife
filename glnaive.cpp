#include "glnaive.h"

#include <QBitmap>
#include <QImage>
#include <QString>
#include <random>
#include <chrono>

#include "universe.h"

GLNaive::GLNaive(): GLAlgo(),
    mMat(new Universe(mHeight, mWidth))
{
    mMat->clear();
}

GLNaive::~GLNaive()
{

}

QString GLNaive::name() const
{
    return "Naive";
}

void GLNaive::setSize(unsigned width, unsigned height)
{
    mWidth = width;
    mHeight = height;
    mMat->resize(mHeight, mWidth);
    mMat->clear();
    mSize = mMat->lengthInBits();
}

void GLNaive::reset()
{
    mMat->clear();
    mGeneration = 0;
}

void GLNaive::nextGen()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    size_t size = mMat->lengthInBits();
    Universe& bm = *mMat;
    std::unique_ptr<Universe> next_mat(new Universe(mHeight, mWidth));
    Universe& next = *next_mat;
    next.clear();
    mPopulation = 0;
    char (Universe::*fn)(unsigned long, int);
    switch (mBorderRule) {
    case BorderRule::CLOSING:
        fn = &Universe::countMooreNeighborsBordersClosing;
        break;
    case BorderRule::ALIVE:
        fn = &Universe::countMooreNeighborsBordersAlive;
        break;
    case BorderRule::DEAD:
        fn = &Universe::countMooreNeighborsBordersDead;
        break;
    default:
        fn = &Universe::countMooreNeighborsBordersClosing;
    }

    for (size_t i = 0; i < size; i++) {
        int count = (*mMat.*fn)(i, 4);
        if (count == 3 || (count == 2 && bm[i])) {
            next.set(i, true);
            mPopulation++;
        }
    }
    mMat.swap(next_mat);
    mGeneration++;
    auto t2 = std::chrono::high_resolution_clock::now();
    mLastGenerationTime = static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count());
}

void GLNaive::insertSample(const QBitmap &other, unsigned x, unsigned y)
{
    Universe& bm = *mMat;
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
    return QBitmap::fromData(QSize(static_cast<int>(mWidth), static_cast<int>(mHeight)), mMat->data(), QImage::Format::Format_Mono);
}

bool GLNaive::isAlive(unsigned x, unsigned y) const
{
    size_t idx = x + y * mWidth;
    return (*mMat)[idx];
}

void GLNaive::toggle(unsigned x, unsigned y)
{
    size_t idx = x + y * mWidth;
    mMat->set(idx, !(*mMat)[idx]);
}



void GLNaive::randomize()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> rg;
    size_t len = mMat->lengthInBits() / sizeof (int) / 8;
    unsigned* p_mat_ints = reinterpret_cast<unsigned*>(mMat->data());
    mPopulation = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned val = rg(gen);
        for (unsigned i = 0; i < sizeof(unsigned)*8; i++)
            if (val & (1 < i)) mPopulation++;
        p_mat_ints[i] = val;
    }
    mGeneration = 0;
}
