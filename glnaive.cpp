#include "glnaive.h"

#include <QBitmap>
#include <QImage>
#include <QString>
#include <random>
#include <chrono>

GLNaive::GLNaive(): GLAlgo(),
    mMat(new bitmatrix_t(mHeight, mWidth))
{

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
    mSize = mMat->lengthInBits();
}

void GLNaive::reset()
{
    mMat->clear();
    mGeneration = 0;
}

int has_bits_in_char(const char in, const int count)
{
    int c = 0, i =0;
    while (i < 8 && c < count) {
        if (in & (1<<i)) c++;
        i++;
    }
    return c;
}

void GLNaive::nextGen()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    size_t size = mMat->lengthInBits();
    bitmatrix_t& bm = *mMat;
    std::unique_ptr<bitmatrix_t> next_mat(new bitmatrix_t(mHeight, mWidth));
    bitmatrix_t& next = *next_mat;
    mPopulation = 0;
    for (size_t i = 0; i < size; i++) {
        char ns = neighbors(i);
        int count = has_bits_in_char(ns, 4);
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
    bitmatrix_t& bm = *mMat;
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
    return QBitmap::fromData(QSize(static_cast<int>(mWidth), static_cast<int>(mHeight)), static_cast<uchar*>(mMat->data()), QImage::Format::Format_Mono);
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> rg;
    size_t len = mMat->lengthInBits() / sizeof (int) / 8;
    unsigned* p_mat_ints = static_cast<unsigned*>(mMat->data());
    mPopulation = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned val = rg(gen);
        for (unsigned i = 0; i < sizeof(int)*8; i++)
            if (val & (1 < i)) mPopulation++;
        p_mat_ints[i] = val;
    }
    mGeneration = 0;
}

size_t inline smod(long a, size_t b)
{
    return a < 0 ? b - static_cast<size_t>(-a) : static_cast<size_t>(a) % b;
}

char GLNaive::neighbors(size_t idx) const
{
    bool out[8];
    long _idx = static_cast<long>(idx) - mWidth - 1;
    bitmatrix_t& mat = *mMat;
    out[0] = mat[smod(_idx, mSize)];
    out[1] = mat[smod(++_idx, mSize)];
    out[2] = mat[smod(++_idx, mSize)];
    out[3] = mat[smod(static_cast<long>(idx) + 1, mSize)];
    _idx = static_cast<long>(idx) + mWidth + 1;
    out[4] = mat[smod(_idx, mSize)];
    out[5] = mat[smod(--_idx, mSize)];
    out[6] = mat[smod(--_idx, mSize)];
    out[7] = mat[smod(static_cast<long>(idx) - 1, mSize)];
    char result {0};
    for (int i = 0; i < 8; i++)
        if (out[i]) result = result | (0x80 >> i);
    return result;
}
