#ifndef IMAGES_H
#define IMAGES_H

#include <functional>
#include <iostream>
#include "coordinate.h"
#include "color.h"


namespace Details {
		inline Point add_vector(const Point p, const Vector& v, int scale) {
				return Point(p.first + v.first * scale, p.second + v.second * scale);
		}

		inline auto translator(const Vector v, int scale) {
				return [v, scale](const Point p){ return add_vector(p, v, scale); };
		}

		inline Point scale(const Point& p, double s) {
				return Point(p.first * s, p.second * s, p.is_polar);
		}

		inline auto scaler(double s) {
				return [=](const Point p){ return Details::scale(p, 1.0 / s); };
		}

		inline Point rotate(const Point& p, double phi) {
				auto new_x = p.first * cos(phi) - p.second * sin(phi);
				auto new_y = p.second * cos(phi) + p.first * sin(phi);
				return Point(new_x, new_y, p.is_polar);
		}

		 inline auto rotator(double phi) {
				return [phi](const Point p){
						return Details::rotate(p, phi);
				};
		}
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
		return compose(Details::rotator(-phi), image);
}


template<typename T>
inline Base_image<T> translate(Base_image<T> image, Vector v) {
		return compose(Details::translator(v, -1), image);
}


template<typename T>
inline Base_image<T> scale(Base_image<T> image, double s) {
		return compose(Details::scaler(s), image);
}


template<typename T>
inline Base_image<T> circle(Point q, double r, T inner, T outer) {
		return [=](const Point p){ return distance(q, p) <= r ? inner : outer; };
}


template<typename T>
inline Base_image<T> checker(double d, T this_way, T that_way) {
		return [=](const Point p){
				auto x_units = static_cast<int64_t>(std::floor(p.first / d));
				auto y_units = static_cast<int64_t>(std::floor(p.second / d));
				return (x_units + y_units) % 2 ? that_way : this_way;
		};
}


template<typename T>
inline Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
		return constant(this_way);
}


template<typename T>
inline Base_image<T> rings(Point q, double d, T this_way, T that_way) {
		return [=](const Point p){
				auto dist_units = static_cast<uint64_t>(distance(q, p) / d );
				return dist_units % 2 ? that_way : this_way;
		};
}


template<typename T>
inline Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
		return [=](const Point p){
				return 2 * std::abs(p.first) <= d ? this_way : that_way;
		};
}

Image cond(Region region, Image this_way, Image that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);


#endif //IMAGES_H
