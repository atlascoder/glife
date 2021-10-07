#include "sample.h"

#include <QUuid>
#include <QDebug>

const QString Sample::LIFE_RULE = "B3/S23";

Sample::Sample():
    mName("Unnamed"),
    mWidth(0),
    mHeight(0),
    mBitmap(0,0),
    mRule(LIFE_RULE),
    mId(QUuid::createUuid().toString(QUuid::WithoutBraces))
{ }

Sample::Sample(const QString& name, int width, int height, const QBitmap& bitmap, const QString& rule, const QString& id):
    mName(name),
    mWidth(width),
    mHeight(height),
    mBitmap(bitmap),
    mRule(rule),
    mId(id)
{ }

Sample::Sample(const Sample& sample):
    mName(sample.mName),
    mWidth(sample.mWidth),
    mHeight(sample.mHeight),
    mBitmap(sample.mBitmap),
    mRule(sample.mRule),
    mId(sample.mId)
{ }

Sample::Sample(Sample&& other):
    mName(std::move(other.mName)),
    mWidth(other.mWidth),
    mHeight(other.mHeight),
    mBitmap(std::move(other.mBitmap)),
    mRule(std::move(other.mRule)),
    mId(std::move(other.mId))
{ }

Sample& Sample::operator=(const Sample& other)
{
    mName = other.mName;
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mBitmap = other.mBitmap;
    mRule = other.mRule;
    mId = other.mId;
    qDebug() << "Copy Assignment for ID=[" << mId << "]";
    return *this;
}

Sample& Sample::operator=(Sample &&other)
{
    qDebug() << "Move assignment for ID=[" << mId << "]";
    mName = std::move(other.mName);
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mBitmap = std::move(other.mBitmap);
    mRule = std::move(other.mRule);
    mId = std::move(other.mId);
    return *this;
}

Sample::~Sample()
{ }
