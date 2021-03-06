#ifndef IMAGES_H
#define IMAGES_H

#include <functional>
#include <iostream>
#include <cassert>
#include "color.h"
#include "coordinate.h"


namespace Details {
// Przekształcenie geometryczne obrazu (Point -> Point).
// Przekształca punkt z wynikowej powierzchni na punkt ze źródłowej powierzchni.
    using Transform = std::function<Point(const Point)>;

// Przesunięcie płaszczyzny o zadany Vector.
    Transform translate_point(Vector v);

// Przeskalowanie płaszczyzny o współczynnik s (względem punktu (0, 0)).
    Transform scale_point(double s);

// Skalowanie kąta punktu o współrzędnych polarnych.
    Transform sc(int n, double d);

// Obrót punktu o koordynatahc polarnych względem środka obrazu o kąt phi.
    Transform rotate_polar(double phi);
}  // namespace Details

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
inline Base_image<T> constant(const T t) {
    return [=](const Point) { return t; };
}

template<typename T>
inline Base_image<T> rotate(Base_image<T> image, double phi) {
    return compose(to_polar, Details::rotate_polar(phi), from_polar, image);
}

template<typename T>
inline Base_image<T> translate(Base_image<T> image, Vector v) {
    return compose(Details::translate_point(v), image);
}

template<typename T>
inline Base_image<T> scale(Base_image<T> image, double s) {
    assert(s != 0);
    return compose(Details::scale_point(s), image);
}

template<typename T>
inline Base_image<T> circle(Point q, double r, T inner, T outer) {
    return [=](const Point p) { return distance(q, p) <= r ? inner : outer; };
}

template<typename T>
inline Base_image<T> checker(double d, T this_way, T that_way) {
    assert(d != 0);
    return [=](const Point p) {
        auto x_units = static_cast<int64_t>(std::floor(p.first / d));
        auto y_units = static_cast<int64_t>(std::floor(p.second / d));
        return (x_units + y_units) % 2 ? that_way : this_way;
    };
}

template<typename T>
inline Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    assert(d != 0);
    return compose(to_polar, Details::sc(n, d), checker(d, this_way, that_way));
}

template<typename T>
inline Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    assert(d != 0);
    return [=](const Point p) { return static_cast<uint64_t>(distance(q, p) / d) % 2 ? that_way : this_way; };
}

template<typename T>
inline Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [=](const Point p) { return 2 * std::abs(p.first) <= d ? this_way : that_way; };
}

Image cond(const Region& region, const Image& this_way, const Image& that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);

#endif  // IMAGES_H
