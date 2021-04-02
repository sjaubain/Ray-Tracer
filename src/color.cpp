#include "color.h"

Color::Color() {
    rgb_format.r = rgb_format.g = rgb_format.b = MIN;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) {
    rgb_format.r = r; rgb_format.g = g, rgb_format.b = b;
}

Color::Color(uint32_t rgb) {
    rgb_format.r = rgb >> 16 & 0xFF; rgb_format.g = rgb >> 8 & 0xFF, rgb_format.b = rgb & 0xFF;
}

Color Color::operator * (float lambda) const {return Color(lambda * rgb_format.r,
                                                           lambda * rgb_format.g,
                                                           lambda * rgb_format.b);}

Color operator * (float lambda, const Color& c) {return c * lambda;}

Color Color::operator + (const Color& c) const {return Color((int)(rgb_format.r + c.rgb_format.r) % (MAX + 1),
                                                             (int)(rgb_format.g + c.rgb_format.g) % (MAX + 1),
                                                             (int)(rgb_format.b + c.rgb_format.b) % (MAX + 1));}
rgb_t Color::getRgbFormat() const {return rgb_format;}

void Color::print() const {std::cout << "[R : " << (int)rgb_format.r
                                     << " G : " << (int)rgb_format.g
                                     << " B : " << (int)rgb_format.b << "]";}

Color Color::mixWith(const Color& c, float alpha) const {
    return Color(rgb_format.r,
                 rgb_format.g * alpha + (1 - alpha) * c.rgb_format.g,
                 rgb_format.b * alpha + (1 - alpha) * c.rgb_format.b);
}

const Color Color::red(MAX, 0, 0);
const Color Color::green(0, MAX, 0);
const Color Color::blue(0, 0, MAX);
const Color Color::white(MAX, MAX, MAX);
const Color Color::black(0, 0, 0);
const Color Color::sky(136, 233, 252);
const Color Color::darksky(101, 172, 186);
