#include "rlereader.h"

#include <QFile>
#include <QStringBuilder>
#include <QTextStream>
#include <cstring>

RleReader::RleReader(const QString& filePath):
    mContent(),
    mRule(),
    mName(filePath.contains("/") ? filePath.split("/").last() : filePath),
    mErrorMessage(),
    mWidth(0),
    mHeight(0),
    mIsValid(false),
    mBitmap()
{
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        mContent = in.readAll();
        file.close();
        parseContent();
    }
    else {
        mErrorMessage = file.errorString();
    }
}

inline void set_bit_in_bytearr(uint8_t* arr, int pos)
{
    arr[pos/8] = arr[pos/8] | (0x80 >> pos%8);
}

void RleReader::parseContent()
{
    QStringList lines(mContent.split(QRegExp("\\r?\\n")));
    QStringList::ConstIterator l = lines.constBegin();

    // iterate over comment fields
    while (l->startsWith("#") && l != lines.constEnd()) {
        if (mName.isNull() && l->startsWith("#N")) {
            mName = l->mid(2).trimmed();
        }
        l++;
    }

    if (l == lines.constEnd()) {
        mErrorMessage = "RLE file header absent";
        return;
    }

    // parsing header
    QRegExp header_re("\\s?x\\s?=\\s?(\\d+),\\s?y\\s?=\\s?(\\d+)(,\\s?rule\\s?=\\s?([\\w\\/]*))?.*");

    if (header_re.indexIn(*l) != -1) {
        bool parsed;
        mWidth = header_re.cap(1).toInt(&parsed);

        if (!parsed) {
            mErrorMessage = "x has invalid value";
            return;
        }

        mHeight = header_re.cap(2).toInt(&parsed);

        if (!parsed) {
            mErrorMessage = "y has invalid value";
            return;
        }

        if (header_re.captureCount() > 3) {
            mRule = header_re.cap(3);
        }
        else {
            mRule = "B3/S23";
        }
    }
    else {
        mErrorMessage = "invalid RLE header";
        return;
    }

    l++;

    if (l == lines.constEnd()) {
        mErrorMessage = "empty content after RLE header";
        return;
    }

    QString rlecode;

    bool rlecode_terminated = false;

    while (l != lines.constEnd() && !rlecode_terminated) {
        if (l->contains("!")) {
            rlecode.append(l->midRef(0, l->indexOf("!")));
            rlecode_terminated = true;
            break;
        }
        else {
            rlecode.append(*l);
            l++;
        }
    }

    if (!rlecode_terminated || !rlecode.length()) {
        mErrorMessage = "wrong RLE content";
        return;
    }

    int bytes_width = mWidth / 8 + (mWidth % 8 != 0);
    unsigned bytes_size = static_cast<unsigned>(bytes_width) * static_cast<unsigned>(mHeight);


    std::vector<uint8_t> bytes(bytes_size);
    std::memset(bytes.data(), 0, bytes_size);

    QStringList rle_lines = rlecode.split("$");

    int rle_line_len = rle_lines.length();

    int rle_line_idx = 0;
    unsigned bytes_line = 0;

    while (rle_line_idx < rle_line_len) {
        int i = 0;
        auto& rle_line = rle_lines[rle_line_idx];
        int l = rle_line.length();
        int pos = 0;
        uint8_t* bitmap_line = static_cast<uint8_t*>(bytes.data()) + static_cast<unsigned>(bytes_width) * bytes_line;
        int count = 0;

        while(i < l) {
            int ns = i, nl = 0;
            while (i < l && rle_line[i].isDigit()) {
                nl++;
                i++;
            }
            count = nl ? rle_line.midRef(ns, nl).toInt() : 1;
            if (i == l) break;
            if (rle_line[i] == 'o') {
                for (int j = 0; j < count; j++) set_bit_in_bytearr(bitmap_line, pos + j);
                pos += count;
                count = 0;
            }
            else if (rle_line[i] == 'b') {
                pos += count;
                count = 0;
            }
            i++;
        }

        bytes_line += count ? static_cast<unsigned>(count) : 1;
        rle_line_idx++;
    }

    mBitmap = QBitmap::fromData(QSize(static_cast<int>(mWidth), static_cast<int>(mHeight)), bytes.data(), QImage::Format::Format_Mono);
    mIsValid = true;
}
