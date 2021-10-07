#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>
#include <QBitmap>

class Sample
{
protected:
    QString mName;
    int mWidth;
    int mHeight;
    QBitmap mBitmap;
    QString mRule;
    QString mId;
public:
    static const QString LIFE_RULE;

    // default constructor
    Sample();

    // parametrized constr
    Sample(const QString& name, int width, int height, const QBitmap& bitmap, const QString& rule, const QString& id);

    // copy constructor
    Sample(const Sample& other);

    // move constructor
    Sample(Sample&& other);

    Sample& operator=(const Sample& other);

    Sample& operator=(Sample&& other);

    ~Sample();

    QString name() const { return mName; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    QBitmap bitmap() const { return mBitmap; }
    QString rule() const { return mRule; }
    QString id() const { return mId; }

};

#endif // SAMPLE_H
