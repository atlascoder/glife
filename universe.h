#ifndef UNIVERSE_H
#define UNIVERSE_H

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

    char nsWithClosingB(unsigned row, unsigned col);
    char nsWithDeadB(unsigned row, unsigned col);
    char nsWithAliveB(unsigned row, unsigned col);

    void set(unsigned row, unsigned col, bool value);
    void resize(unsigned rows, unsigned cols);
    void clear();
    unsigned char* data() const;
    unsigned rows() const;
    unsigned cols() const;
    unsigned rowLengthAligned() const;
    unsigned lengthInStorageType() const;
    unsigned long lengthInBits() const;

private:
    unsigned mRows;
    unsigned mCols;
    unsigned mRowLengthAligned;
    unsigned long mBits;
    unsigned mLength;
    unsigned char* mVector;
};

#endif // UNIVERSE_H
