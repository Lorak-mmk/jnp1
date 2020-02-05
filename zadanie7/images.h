#ifndef IMAGES_H
#define IMAGES_H

#include <functional>
#include "coordinate.h"
#include "color.h"


namespace Details {
}


using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;


template<typename T>
inline Base_image<T> constant(const T& t) {
		return [=](const Point) { return t; };
}


template<typename T>
inline Base_image<T> rotate(Base_image<T> image, double phi) {
	return image;
}


template<typename T>
inline Base_image<T> translate(Base_image<T> image, Vector v) {
		return image;
}


template<typename T>
inline Base_image<T> scale(Base_image<T> image, double s) {
		return image;
}


template<typename T>
inline Base_image<T> circle(Point q, double r, T inner, T outer) {
		return [=](const Point p){ return distance(q, p) <= r ? inner : outer; };
}


template<typename T>
inline Base_image<T> checker(double d, T this_way, T that_way) {
	return constant(this_way);
}


template<typename T>
inline Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
	return constant(this_way);
}


template<typename T>
inline Base_image<T> rings(Point q, double d, T this_way, T that_way) {
		return [=](const Point p){
				auto x = static_cast<uint64_t>(distance(q, p) / (2 * d) );
				return x % 2 ? this_way : that_way;
		};
}


template<typename T>
inline Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
		return [=](const Point p){
				return std::abs(p.first) / 2 <= d ? this_way : that_way;
		};
}

Image cond(Region region, Image this_way, Image that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);


#endif //IMAGES_H
