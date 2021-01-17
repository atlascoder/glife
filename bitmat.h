#ifndef BITMATRIX_H
#define BITMATRIX_H

#include <memory>
#include <vector>
#include <cstring>
/*
 *  `bitmap` is a custom implemetation of matrix of bits.
 *  T is a the real storage type that can vary.
 *  `rows` and `cols` can be abitrary, but the matrix
 *  is always broking down into sequence of rows of T
 *  and the rows of T length is aligned by T length.
 */

template <class T>
class bitmat
{
    static constexpr size_t const& bits_in_t = 8 * sizeof(T);
    static constexpr T const& lsb_1 = T(1) << (bits_in_t - 1);
public:

    bitmat(unsigned rows, unsigned cols):
        mRows(rows),
        mCols(cols),
        mRowLengthAligned(mCols / bits_in_t + (mCols % bits_in_t != 0)),
        mBits(mRows * mCols),
        mLength(mRows * mRowLengthAligned),
        mVector(new std::vector<T>(mLength))
    {}

    bitmat(const bitmat& other):
        mRows(other.mRows),
        mCols(other.mCols),
        mRowLengthAligned(other.mRowLengthAligned),
        mBits(other.mBits),
        mLength(other.mLength),
        mVector(new std::vector<T>(*other.mVector))
    {}

    bitmat(bitmat&& other):
        mRows(other.mRows),
        mCols(other.mCols),
        mRowLengthAligned(other.mRowLengthAligned),
        mBits(other.mBits),
        mLength(other.mLength),
        mVector(std::exchange(other.mVector, nullptr))
    {}

    bitmat& operator=(const bitmat& other)
    {
        return *this = bitmat<T>(other);
    }

    bitmat& operator=(bitmat&& other)
    {
        mRows = other.mRows;
        mCols = other.mCols;
        mRowLengthAligned = other.mRowLengthAligned;
        mBits = other.mBits;
        mLength = other.mLength;
        mVector.swap(other.mVector);
        return *this;
    }

    T* row(unsigned rowIdx) {
        return mVector->data() + rowIdx * mRowLengthAligned;
    }

    bool operator[](size_t idx)
    {
        unsigned row_idx = idx / mCols;
        return row(row_idx)[(idx % mCols) / bits_in_t] & (lsb_1 >> idx % bits_in_t);
    }

    bool get(unsigned row, unsigned col) const {
        return (*this)[row * mCols + col];
    }

    void set(size_t idx, bool value) {
        unsigned row_idx = idx / mCols;
        T* s = row(row_idx) + (idx % mCols) / bits_in_t;
        T mask = lsb_1 >> idx % bits_in_t;
        if (value) {
            *s |= mask;
        }
        else {
            *s ^= mask;
        }
    }

    void set(unsigned row, unsigned col, bool value) {
        T* s = this->row(row) + col / bits_in_t;
        T mask = lsb_1 >> col % bits_in_t;
        if (value) {
            *s |= mask;
        }
        else {
            *s ^= mask;
        }
    }

    void resize(unsigned rows, unsigned cols) {
        if (rows == mRows && cols == mCols) return;
        *this = std::move(bitmat(rows, cols));
    }

    void clear()
    {
        std::memset(mVector->data(), 0, mVector->size() * sizeof (T));
    }

    void* data() const { return mVector->data(); }
    unsigned rows() const { return mRows; }
    unsigned cols() const { return mCols; }
    unsigned rowLengthAligned() const { return mRowLengthAligned; }
    size_t lengthInStorageType() const { return mLength; }
    size_t lengthInBits() const { return mBits; }

private:
    unsigned mRows;
    unsigned mCols;
    unsigned mRowLengthAligned;
    size_t mBits;
    size_t mLength;
    std::unique_ptr<std::vector<T>> mVector;
};

#endif // BITMATRIX_H
