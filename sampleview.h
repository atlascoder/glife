#ifndef SAMPLEVIEW_H
#define SAMPLEVIEW_H

#include <QQuickPaintedItem>
#include <QBitmap>

class SampleView: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows NOTIFY bitmapChanged)
    Q_PROPERTY(int cols READ cols NOTIFY bitmapChanged)
    Q_PROPERTY(QBitmap bitmap READ bitmap WRITE setBitmap NOTIFY bitmapChanged)

    void paint(QPainter* painter) override;
public:
    SampleView(QQuickItem* parent = nullptr);

    int rows() const { return mBitmap.height(); }
    int cols() const { return mBitmap.width(); }
    QBitmap bitmap() const { return mBitmap; }

    void setBitmap(const QBitmap& value);

signals:
    void bitmapChanged();
private:
    QBitmap mBitmap;

};

#endif // SAMPLEVIEW_H
