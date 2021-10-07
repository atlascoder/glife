#include "extrlesampleconverter.h"

Sample ExtRLESampleConverter::fromSampleData(const SampleData &data)
{
    QString name = data.id, rule, id = data.id;
    int width, height;
    QBitmap bitmap;
    mError.clear();
    try {
        QString content(data);
        QStringList lines(content.split(QRegExp("\\r?\\n")));

        QStringList::ConstIterator l = lines.constBegin();

        // iterate over comment fields
        while (l->startsWith("#") && l != lines.constEnd()) {
            if (name.isEmpty() && l->startsWith("#N")) {
                name = l->mid(2).trimmed();
            }
            l++;
        }

        if (l == lines.constEnd()) {
            mError = "RLE file header absent";
            throw;
        }

        // parsing header
        QRegExp header_re("\\s?x\\s?=\\s?(\\d+),\\s?y\\s?=\\s?(\\d+)(,\\s?rule\\s?=\\s?([\\w\\/]*))?.*");

        if (header_re.indexIn(*l) != -1) {
            bool parsed;
            width = header_re.cap(1).toInt(&parsed);

            if (!parsed) {
                mError = "x has invalid value";
                throw;
            }

            height = header_re.cap(2).toInt(&parsed);

            if (!parsed) {
                mError = "y has invalid value";
                throw;
            }

            if (header_re.captureCount() > 3) {
                rule = header_re.cap(3);
            }
            else {
                rule = "B3/S23";
            }
        }
        else {
            mError = "invalid RLE header";
            throw;
        }

        l++;

        if (l == lines.constEnd()) {
            mError = "empty content after RLE header";
            throw;
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
            mError = "wrong RLE content";
            throw;
        }

        int bytes_width = width / 8 + (width % 8 != 0);
        unsigned bytes_size = static_cast<unsigned>(bytes_width) * static_cast<unsigned>(height);


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
                    for (int j = 0; j < count; j++) {
                        int p = pos + j;
                        bitmap_line[p/8] = bitmap_line[p/8] | (0x80 >> p%8);
                    }
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

        bitmap = QBitmap::fromData(QSize(static_cast<int>(width), static_cast<int>(height)), bytes.data(), QImage::Format::Format_Mono);
    }
    catch (...) {
        return Sample();
    }
    return Sample(name, width, height, bitmap, rule, id);
}

SampleData ExtRLESampleConverter::toSampleData(const Sample &sample)
{
    SampleData sd;
    sd.id = sample.id();
    mError = "Not implemented";

    return sd;
}
