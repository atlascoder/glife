#ifndef BITMATRIX_H
#define BITMATRIX_H

#include "borderrule.h"

class Universe
{
public:

    Universe(unsigned rows, unsigned cols);
    Universe(const Universe& other);
    Universe(Universe&& other);

    ~Universe();

    Universe& operator=(const Universe& other);
    Universe& operator=(Universe&& other);
    bool operator[](unsigned long idx);

    unsigned char* row(unsigned rowIdx);
    void set(unsigned long idx, bool value);

    char countMooreNeighborsBordersClosing(unsigned long idx, int stopOn);
    char countMooreNeighborsBordersDead(unsigned long idx, int stopOn);
    char countMooreNeighborsBordersAlive(unsigned long idx, int stopOn);

    void set(unsigned row, unsigned col, bool value);
    void resize(unsigned rows, unsigned cols);
    void clear();
    unsigned char* data() const;
    unsigned rows() const;
    unsigned cols() const;
    unsigned rowLengthAligned() const;
    unsigned lengthInStorageType() const;
    unsigned long lengthInBits() const;

    void setBorderRule(const BorderRule rule);
    BorderRule borderRule() const;

private:
    unsigned mRows;
    unsigned mCols;
    unsigned mRowLengthAligned;
    unsigned long mBits;
    unsigned mLength;
    unsigned char* mVector;
    BorderRule mBorderRule;
};

#endif // BITMATRIX_H
