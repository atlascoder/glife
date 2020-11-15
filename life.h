#ifndef LIFE_H
#define LIFE_H

#include <bitset>

#define CAPACITY 1000000

namespace GLife {

class Life
{
public:
    Life();

    void cycle();

    void toggle(const unsigned row, const unsigned col);

    unsigned rows() const;
    void setRows(const unsigned value);

    unsigned cols() const;
    void setCols(const unsigned value);

    bool isAlive(const unsigned row, const unsigned col) const;
protected:
    std::bitset<8> neighbors(const uint32_t idx) const;

private:
    unsigned m_rows;
    unsigned m_cols;
    unsigned m_size;
    std::bitset<CAPACITY> m_field;

    size_t index(const unsigned row, const unsigned col) const;
};

}

#endif // LIFE_H
