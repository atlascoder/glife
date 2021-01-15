#ifndef RLEREADER_H
#define RLEREADER_H

#include <QString>
#include <QBitmap>

class RleReader
{
public:
    RleReader(const QString& filePath);

    bool isValid() const { return mIsValid;}
    QString rule() const { return mRule; }
    QString name() const { return mName; }
    QString errorMessage() const { return mErrorMessage; }

    int width() const { return mWidth; }
    int height() const { return mHeight; }

    QBitmap bitmap() const { return mBitmap; }

private:
    QString mContent;
    QString mRule;
    QString mName;
    QString mErrorMessage;
    int mWidth;
    int mHeight;
    bool mIsValid;
    QBitmap mBitmap;

    void parseContent();
};

#endif // RLEREADER_H
