#include <utility>
#include "functional.h"
#include "images.h"


namespace Details {
    Transform translate_point(const Vector v) {
        return [=](const Point p) { return Point(p.first - v.first, p.second - v.second); };
    }

    Transform scale_point(double s) {
        return [=](const Point p) { return Point(p.first / s, p.second / s, p.is_polar); };
    }

    Transform sc(int n, double d) {
        return [=](const Point p) { return Point(p.first, p.second * d * n / (2 * M_PI), true); };
    }

    Transform rotate_polar(double phi) {
        return [=](const Point p) { return Point(p.first, p.second - phi, true); };
    }
}  // namespace Details

Image cond(const Region& region, const Image& this_way, const Image& that_way) {
    return [=](const Point p) { return region(p) ? this_way(p) : that_way(p); };
}

Image lerp(Blend blend, Image this_way, Image that_way) {
    return lift(&Color::weighted_mean, this_way, that_way, blend);
}

Image darken(Image image, Blend blend) {
    return lerp(std::move(blend), std::move(image), constant(Colors::black));
}

Image lighten(Image image, Blend blend) {
    return lerp(std::move(blend), std::move(image), constant(Colors::white));
}
