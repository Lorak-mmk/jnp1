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
		return [=](Point) { return t; };
}


template<typename T>
inline Base_image<T> rotate(Base_image<T> image, double phi) {
	return image;
}


template<typename T>
inline Base_image<T> translate(Base_image<T> image, Vector v) {
		return [](Point p){};
}


template<typename T>
inline Base_image<T> scale(Base_image<T> image, double s) {
		return image;
}


template<typename T>
inline Base_image<T> circle(Point q, double r, T inner, T outer) {
		return constant(inner);
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
		return constant(this_way);
}


template<typename T>
inline Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
		return constant(this_way);
}

Image cond(Region region, Image this_way, Image that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);


#endif //IMAGES_H
