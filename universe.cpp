#include "universe.h"

#include <memory>
#include <cstring>

typedef unsigned char uchar;


static const unsigned bits_in_t = 8;
static const unsigned char lsb_1 = 0x80;

Universe::Universe(unsigned rows, unsigned cols):
    mRows(rows),
    mCols(cols),
    mRowLengthAligned(mCols / bits_in_t + (mCols % bits_in_t != 0)),
    mBits(mRows * mCols),
    mLength(mRows * mRowLengthAligned),
    mVector(new unsigned char[mLength])
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
    mVector(new unsigned char[mLength])
{
    std::memcpy(mVector, other.mVector, mLength);
}

Universe::Universe(Universe&& other):
    mRows(other.mRows),
    mCols(other.mCols),
    mRowLengthAligned(other.mRowLengthAligned),
    mBits(other.mBits),
    mLength(other.mLength)
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

inline uchar COUNT_ROWC(const uchar* row, unsigned col_idx, unsigned last_col_idx)
{
    uchar count = 0;
    if (col_idx == 0) {
        // first column
        uchar b1_2 = row[0] >> 6;
        count += b1_2 & 0x01;
        b1_2 >>= 1;
        count += b1_2 & 0x01;
        count += row[last_col_idx / 8] & 0x01;
    }
    else if (col_idx == last_col_idx) {
        // last column
        count += (row[0] >> 7) & 0x01;
        uchar b0_1 = row[last_col_idx / 8];
        count += b0_1 & 0x01;
        b0_1 >>= 1;
        count += b0_1 & 0x01;
    }
    else {
        unsigned b0_idx = col_idx - 1;
        const uchar* bp = row + b0_idx / 8;
        uchar b0_x = bp[0];
        uchar b0_pos = b0_idx % 8;
        if (b0_pos < 6) {
            b0_x >>= 5 - b0_pos;
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
        }
        else if (b0_pos == 6) {
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            count += b1_2 >> 7;
        }
        else {
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            b1_2 >>= 6;
            count += b1_2 & 0x01;
            b1_2 >>= 1;
            count += b1_2 & 0x01;
        }
    }
    return count;
}

inline uchar COUNT_M_ROWC(const uchar* row, unsigned col_idx, unsigned last_col_idx)
{
    uchar count = 0;
    if (col_idx == 0) {
        // first column
        uchar b2 = row[0] >> 6;
        count += b2 & 0x01;
        count += row[last_col_idx / 8] & 0x01;
    }
    else if (col_idx == last_col_idx) {
        // last column
        count += (row[0] >> 7) & 0x01;
        uchar b0_1 = row[last_col_idx / 8];
        b0_1 >>= 1;
        count += b0_1 & 0x01;
    }
    else {
        unsigned b0_idx = col_idx - 1;
        const uchar* bp = row + b0_idx / 8;
        uchar b0_x = bp[0];
        uchar b0_pos = b0_idx % 8;
        if (b0_pos < 6) {
            b0_x >>= 5 - b0_pos;
            count += b0_x & 0x01;
            b0_x >>= 2;
            count += b0_x & 0x01;
        }
        else if (b0_pos == 6) {
            b0_x >>= 1;
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            count += b1_2 >> 7;
        }
        else {
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            b1_2 >>= 6;
            count += b1_2 & 0x01;
        }
    }
    return count;
}

inline uchar COUNT_ROW(const uchar* row, unsigned col_idx, unsigned last_col_idx, bool borderAlive)
{
    uchar count = 0;
    if (col_idx == 0) {
        // first column
        uchar b1_2 = row[0] >> 6;
        count += b1_2 & 0x01;
        b1_2 >>= 1;
        count += b1_2 & 0x01;
        if (borderAlive) count++;
    }
    else if (col_idx == last_col_idx) {
        // last column
        if (borderAlive) count++;
        uchar b0_1 = row[last_col_idx / 8];
        count += b0_1 & 0x01;
        b0_1 >>= 1;
        count += b0_1 & 0x01;
    }
    else {
        unsigned b0_idx = col_idx - 1;
        const uchar* bp = row + b0_idx / 8;
        uchar b0_x = bp[0];
        uchar b0_pos = b0_idx % 8;
        if (b0_pos < 6) {
            b0_x >>= 5 - b0_pos;
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
        }
        else if (b0_pos == 6) {
            count += b0_x & 0x01;
            b0_x >>= 1;
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            count += b1_2 >> 7;
        }
        else {
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            b1_2 >>= 6;
            count += b1_2 & 0x01;
            b1_2 >>= 1;
            count += b1_2 & 0x01;
        }
    }
    return count;
}

inline uchar COUNT_M_ROW(const uchar* row, unsigned col_idx, unsigned last_col_idx, bool borderAlive)
{
    uchar count = 0;
    if (col_idx == 0) {
        // first column
        uchar b2 = row[0] >> 6;
        count += b2 & 0x01;
        if (borderAlive) count++;
    }
    else if (col_idx == last_col_idx) {
        // last column
        if (borderAlive) count++;
        uchar b0_1 = row[last_col_idx / 8];
        b0_1 >>= 1;
        count += b0_1 & 0x01;
    }
    else {
        unsigned b0_idx = col_idx - 1;
        const uchar* bp = row + b0_idx / 8;
        uchar b0_x = bp[0];
        uchar b0_pos = b0_idx % 8;
        if (b0_pos < 6) {
            b0_x >>= 5 - b0_pos;
            count += b0_x & 0x01;
            b0_x >>= 2;
            count += b0_x & 0x01;
        }
        else if (b0_pos == 6) {
            b0_x >>= 1;
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            count += b1_2 >> 7;
        }
        else {
            count += b0_x & 0x01;
            uchar b1_2 = bp[1];
            b1_2 >>= 6;
            count += b1_2 & 0x01;
        }
    }
    return count;
}

char Universe::nsWithClosingB(unsigned row, unsigned col)
{
    // upper row
    unsigned last_col_idx = mCols - 1;
    char count = COUNT_ROWC(mVector + mRowLengthAligned * (row ? row - 1 : mRows - 1), col, last_col_idx);
    if (count >= 4) return count;
    count += COUNT_M_ROWC(mVector + mRowLengthAligned * row, col, last_col_idx);
    if (count >= 4) return count;
    count += COUNT_ROWC(mVector + (row == mRows - 1 ? 0 : mRowLengthAligned * (row + 1)), col, last_col_idx);
    return count;
}

char Universe::nsWithDeadB(unsigned row, unsigned col)
{
    unsigned last_col_idx = mCols - 1;
    char count = 0;

    if (row != 0) {
        count += COUNT_ROW(mVector + mRowLengthAligned*(row - 1), col, last_col_idx, false);
    }
    if (count >= 4) return count;

    count += COUNT_M_ROW(mVector + mRowLengthAligned*row, col, last_col_idx, false);

    if (count >= 4) return count;

    if (row != mRows - 1) {
        count += COUNT_ROW(mVector + mRowLengthAligned*(row + 1), col, last_col_idx, false);
    }
    return count;
}

char Universe::nsWithAliveB(unsigned row, unsigned col)
{
    unsigned last_col_idx = mCols - 1;
    char count = 0;

    if (row == 0) {
        count += 3;
    }
    else {
        count += COUNT_ROW(mVector + mRowLengthAligned*(row - 1), col, last_col_idx, true);
    }
    if (count >= 4) return count;

    count += COUNT_M_ROW(mVector + mRowLengthAligned*row, col, last_col_idx, true);

    if (count >= 4) return count;

    if (row == mRows - 1) {
        count += 3;
    }
    else {
        count += COUNT_ROW(mVector + mRowLengthAligned*(row + 1), col, last_col_idx, true);
    }
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
