#ifndef UNIVERSEVIEW_H
#define UNIVERSEVIEW_H

#include <QQuickPaintedItem>
#include <QBitmap>

class UniverseView: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int originX READ originX WRITE setOriginX NOTIFY originXChanged)
    Q_PROPERTY(int originY READ originY WRITE setOriginY NOTIFY originYChanged)
    Q_PROPERTY(QColor cellColor READ cellColor WRITE setCellColor NOTIFY cellColorChanged)
    Q_PROPERTY(QBitmap bitmap READ bitmap WRITE setBitmap NOTIFY bitmapChanged)
public:
    UniverseView(QQuickItem* parent = nullptr);
    Q_INVOKABLE void setImage(const QBitmap& image);
    Q_INVOKABLE void setRle(const QString& rle, const int width, const int height);
    void paint(QPainter* painter) override;

    int originX() const { return mOriginX; }
    void setOriginX(const int value);

    int originY() const { return mOriginY; }
    void setOriginY(const int value);

    QColor cellColor() const { return mCellColor; }
    void setCellColor(const QColor& color);

    QBitmap bitmap() const { return mBitmap; }
    void setBitmap(const QBitmap& value);

    Q_INVOKABLE void clear();

signals:
    void areaChanged();
    void originXChanged();
    void originYChanged();
    void cellColorChanged();
    void bitmapChanged();
    void scaleFChanged();
private:
    int mOriginX;
    int mOriginY;
    QColor mCellColor;
    QBitmap mBitmap;
};

#endif // UNIVERSEVIEW_H
