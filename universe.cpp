#include "universe.h"

#include <memory>
#include <cstring>


static const unsigned bits_in_t = 8;
static const unsigned char lsb_1 = 0x80;

Universe::Universe(unsigned rows, unsigned cols):
    mRows(rows),
    mCols(cols),
    mRowLengthAligned(mCols / bits_in_t + (mCols % bits_in_t != 0)),
    mBits(mRows * mCols),
    mLength(mRows * mRowLengthAligned),
    mVector(new unsigned char[mLength]),
    mBorderRule(BorderRule::CLOSING)
{}

Universe::~Universe() {
    delete[] mVector;
}

Universe::Universe(const Universe& other):
    mRows(other.mRows),
    mCols(other.mCols),
    mRowLengthAligned(other.mRowLengthAligned),
    mBits(other.mBits),
    mLength(other.mLength),
    mVector(new unsigned char[mLength]),
    mBorderRule(other.mBorderRule)
{
    std::memcpy(mVector, other.mVector, mLength);
}

Universe::Universe(Universe&& other):
    mRows(other.mRows),
    mCols(other.mCols),
    mRowLengthAligned(other.mRowLengthAligned),
    mBits(other.mBits),
    mLength(other.mLength),
    mBorderRule(other.mBorderRule)
{
    mVector = other.mVector;
    other.mVector = nullptr;
}

Universe& Universe::operator=(const Universe& other)
{
    return *this = Universe(other);
}

Universe& Universe::operator=(Universe&& other)
{
    mRows = other.mRows;
    mCols = other.mCols;
    mRowLengthAligned = other.mRowLengthAligned;
    mBits = other.mBits;
    mLength = other.mLength;
    unsigned char* tmp = mVector;
    mVector = other.mVector;
    other.mVector = tmp;
    mBorderRule = other.mBorderRule;
    return *this;
}

unsigned char* Universe::row(unsigned rowIdx) {
    return mVector + rowIdx * mRowLengthAligned;
}

bool Universe::operator[](unsigned long idx)
{
    unsigned row_idx = idx / mCols;
    return row(row_idx)[(idx % mCols) / bits_in_t] & (lsb_1 >> idx % bits_in_t);
}

void Universe::set(unsigned long idx, bool value) {
    unsigned row_idx = idx / mCols;
    unsigned char* s = row(row_idx) + (idx % mCols) / bits_in_t;
    char mask = lsb_1 >> idx % bits_in_t;
    if (value) {
        *s |= mask;
    }
    else {
        *s ^= mask;
    }
}

char Universe::countMooreNeighborsBordersClosing(unsigned long idx, int stopOn)
{
    char count = 0;
    return count;
}

char Universe::countMooreNeighborsBordersDead(unsigned long idx, int stopOn)
{
    char count = 0;
    return count;
}

char Universe::countMooreNeighborsBorderAlive(unsigned long idx, int stopOn)
{
    char count = 0;
    return count;
}

void Universe::set(unsigned row, unsigned col, bool value)
{
    unsigned char* s = this->row(row) + col / bits_in_t;
    char mask = lsb_1 >> col % bits_in_t;
    if (value) {
        *s |= mask;
    }
    else {
        *s ^= mask;
    }
}

void Universe::resize(unsigned rows, unsigned cols)
{
    if (rows == mRows && cols == mCols) return;
    *this = Universe(rows, cols);
}

void Universe::clear()
{
    std::memset(mVector, 0, mLength);
}

unsigned char* Universe::data() const
{
    return mVector;
}

unsigned Universe::rows() const
{
    return mRows;
}

unsigned Universe::cols() const
{
    return mCols;
}

unsigned Universe::rowLengthAligned() const
{
    return mRowLengthAligned;
}

unsigned Universe::lengthInStorageType() const {
    return mLength;
}

unsigned long Universe::lengthInBits() const
{
    return mBits;
}

void Universe::setBorderRule(const BorderRule rule)
{
    if (rule == mBorderRule) return;
    mBorderRule = rule;
}

BorderRule Universe::borderRule() const
{
    return mBorderRule;
}
