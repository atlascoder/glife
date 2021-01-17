#include "universeview.h"

#include <QPainter>
#include <cstring>
#include "glalgo.h"

UniverseView::UniverseView(QQuickItem* parent):
    QQuickPaintedItem(parent),
    mOriginX(0),
    mOriginY(0),
    mBitmap(GLAlgo::defaultWidth, GLAlgo::defaultHeight)
{
    mBitmap.clear();
}

void UniverseView::setImage(const QBitmap &image)
{
    mBitmap = image;
    update();
}

void UniverseView::paint(QPainter *painter)
{
    if (mBitmap.isNull()) {
        painter->fillRect(this->boundingRect(), fillColor());
    }
    else {
        painter->fillRect(this->boundingRect(), fillColor());
        painter->setPen(mCellColor);
        painter->drawPixmap(QRect(mOriginX, mOriginY, mBitmap.width(), mBitmap.height()),mBitmap);
    }
}

inline void set_bit_in_bytearr(uint8_t* arr, int pos)
{
    arr[pos/8] = arr[pos/8] | (0x80 >> pos%8);
}


void UniverseView::setRle(const QString &rle, const int width, const int height)
{
    size_t bytes_width = static_cast<unsigned>(width) / 8 + (width % 8 != 0);

    QStringList rle_lines {rle.split("!")[0].split("$")};

    unsigned rows = static_cast<unsigned>(rle_lines.length());

    size_t bytes_size = bytes_width * static_cast<size_t>(height);

    std::vector<uint8_t> bytes(bytes_size);
    std::memset(bytes.data(), 0, bytes_size);

    unsigned row = 0, bytes_line = 0;

    while (row < rows) {
        int i = 0;
        QString line {rle_lines[static_cast<int>(row)]};
        int l = line.length();
        int pos = 0;
        uint8_t* bitmap_line = static_cast<uint8_t*>(bytes.data()) + bytes_width * bytes_line;
        int count = 0;

        while(i < l) {
            int ns = i, nl = 0;
            while (i < l && line[i].isDigit()) {
                nl++;
                i++;
            }
            count = nl ? line.midRef(ns, nl).toInt() : 1;
            if (i == l) break;
            if (line[i] == 'o') {
                for (int j = 0; j < count; j++) set_bit_in_bytearr(bitmap_line, pos + j);
                pos += count;
                count = 0;
            }
            else if (line[i] == 'b') {
                pos += count;
                count = 0;
            }
            i++;
        }

        bytes_line += count ? static_cast<unsigned>(count) : 1;
        row++;
    }

    mBitmap = QBitmap::fromData(QSize(width,height), bytes.data(), QImage::Format::Format_Mono);
    update();
}

void UniverseView::setOriginX(int value)
{
    if (value == mOriginX) return;
    mOriginX = value;
    emit originXChanged();
    update();
}

void UniverseView::setOriginY(int value)
{
    if (value == mOriginY) return;
    mOriginY = value;
    emit originYChanged();
    update();
}

void UniverseView::setCellColor(const QColor &color)
{
    if (color.value() == mCellColor.value()) return;
    mCellColor = color;
    emit cellColorChanged();
    update();
}

void UniverseView::setBitmap(const QBitmap &value)
{
    mBitmap = value;
    update();
}

void UniverseView::clear()
{
    mBitmap.clear();
    update();
}
