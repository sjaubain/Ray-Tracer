#include <iostream>

#ifndef COLOR_H
#define COLOR_H

typedef struct rgb {
    uint8_t r; uint8_t g; uint8_t b;
} rgb_t;

class Color {

public:

    Color();
    Color(uint8_t, uint8_t, uint8_t);
    Color(uint32_t);

    friend Color operator * (float, const Color&); // in order to use lambda as left operand
    Color operator * (float) const;
    Color operator + (const Color&) const;
    rgb_t getRgbFormat() const;

    static const uint8_t MIN = 0x00; static const uint8_t MAX = 0xFF;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color white;
    static const Color black;

private:

    rgb_t rgb_format;
};

#endif
