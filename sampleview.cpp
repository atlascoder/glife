#include "sampleview.h"

#include <QBitmap>
#include <QPainter>

SampleView::SampleView(QQuickItem* parent):
    QQuickPaintedItem(parent),
    mBitmap()
{

}

void SampleView::paint(QPainter *painter)
{
    painter->drawPixmap(QRect(0, 0, width(), height()), mBitmap);
}

void SampleView::setBitmap(const QBitmap& value)
{
    mBitmap = value;
    emit bitmapChanged();
}
