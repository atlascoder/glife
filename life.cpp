#include "life.h"

#include <QDebug>

using namespace GLife;

#define DEFAULT_ROWS    20
#define DEFAILT_COLS    10

// corrction of mod for negative a
size_t inline smod(long a, unsigned b)
{
    return static_cast<size_t>(a >= 0 ? a % b : (b + a%b) % b);
}

Life::Life():
    m_rows(DEFAULT_ROWS),
    m_cols(DEFAILT_COLS),
    m_size(DEFAULT_ROWS*DEFAILT_COLS)
{
    for (unsigned i = 0; i < m_size / sizeof(int); i++) {
        int r = rand();
        for (unsigned n = 0; n < sizeof(int); n++) {
            m_field.set(i*sizeof(int) + n, r & (1<<n));
        }
    }

    qDebug() << smod(-5, 3);
}

void Life::cycle()
{
    std::bitset<CAPACITY> stack;
    for (uint32_t idx = 0; idx < m_size; idx++) {
        std::bitset<8> ns = neighbors(idx);
        size_t count = ns.count();
        stack[idx] = count == 3 || (count == 2 && m_field[idx]);
    }
    std::swap(m_field, stack);
}

std::bitset<8> Life::neighbors(const uint32_t idx) const
{
    std::bitset<8> out;
//    unsigned col = idx % m_cols;
//    unsigned row = idx / m_cols;
    long _idx = static_cast<long>(idx) - m_cols - 1;
    out[0] = m_field[smod(_idx, m_size)];
    out[1] = m_field[smod(++_idx, m_size)];
    out[2] = m_field[smod(++_idx, m_size)];
    out[3] = m_field[smod(idx + 1, m_size)];
    _idx = idx + m_cols + 1;
    out[4] = m_field[smod(_idx, m_size)];
    out[5] = m_field[smod(--_idx, m_size)];
    out[6] = m_field[smod(--_idx, m_size)];
    out[7] = m_field[smod(idx - 1, m_size)];
//    out[0] = row > 0 && col > 0 && m_field[idx - m_cols - 1];
//    out[1] = row > 0 && m_field[idx - m_cols];
//    out[2] = row > 0 && col < m_cols-1 && m_field[idx - m_cols + 1];
//    out[3] = col != m_cols - 1 && m_field[idx + 1];
//    out[4] = row != m_rows - 1 && col != m_cols - 1 && m_field[idx + m_cols + 1];
//    out[5] = row != m_rows - 1 && m_field[idx + m_cols];
//    out[6] = row != m_rows - 1 && col > 0 && m_field[idx + m_cols - 1];
//    out[7] = col > 0 && m_field[idx - 1];
    return out;
}

void Life::toggle(const unsigned row, const unsigned col)
{
    auto idx = index(row, col);
    m_field[idx] = ~m_field[idx];
}

unsigned Life::rows() const
{
    return m_rows;
}

void Life::setRows(const unsigned int value)
{
    m_rows = value;
    m_size = m_cols * m_rows;
}

void Life::setCols(const unsigned int value)
{
    m_cols = value;
    m_size = m_cols * m_rows;
}

unsigned Life::cols() const
{
    return m_cols;
}

bool Life::isAlive(const unsigned row, const unsigned col) const
{
    return m_field[index(row,col)] == 1;
}

size_t Life::index(const unsigned row, const unsigned col) const
{
    return m_cols * row + col;
}
