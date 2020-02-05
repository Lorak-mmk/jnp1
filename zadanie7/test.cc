/*
 * AUTOR TESTÓW: KACPER KLUK
 */


#include "functional.h"
#include "images.h"
#include "bmp.h"
#include "color.h"
#include "coordinate.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

using Transformation = std::function<Point(const Point)>;

Point rotate(const Point p, double d) {
	return Point(p.first * std::cos(d) - p.second * std::sin(d),
				 p.first * std::sin(d) + p.second * std::cos(d));
}

int main()
{
	const uint32_t width = 1920;
	const uint32_t height = 1080;


	const Blend cb = [](Point p){ return 1. / (1. + distance(p) / 100.); };
	const Image chck = checker(32., Color(0x012345), Color(0x6789ab));
	const Image pchck = polar_checker(32., 6, Color(0xcdef01), Color(0x234567));

	create_BMP("boiii.bmp", width, height, lerp(cb, chck, pchck));


	const Image vs = vertical_stripe(200., Color(0x89abcd), Color(0xef0123));
	const Transformation twirl = [](Point p){
		return rotate(p, distance(p) / 100.);
	};

	create_BMP("wee-woo.bmp", width, height, compose(twirl, vs));


	const Region rb = rings(Point(-500., 0.), 121., Fraction(0.), Fraction(1.));
	std::vector<Region> rbv;
	for (int i = 0; i < 7; i++) {
		rbv.push_back(rotate(rb, i * 2. * M_PI / 7.));
	}
	Region xorb = constant(false);
	for (int i = 0; i < 7; i++) {
		xorb = [=](Point p){ return xorb(p) ^ rbv[i](p); };
	}

	create_BMP("wot_in_tarnation.bmp", width, height,
		cond(xorb, constant(Color(0x456789)), constant(Color(0xabcdef))));


	const Blend srb = [](Point p){
		return Fraction((1 + std::cos(distance(p) / 175.)) / 2);
	};
	const Transformation swirl = [](Point p){
		double r = to_polar(p).first;
		double phi = to_polar(p).second;
		return from_polar(Point(r * std::cos(phi * 9), phi, true));
	};

	create_BMP("waaah.bmp", width, height, lerp(compose(swirl, srb),
		constant(Color(0xfedcba)), constant(Color(0x987654))));


	const Region vsr = vertical_stripe(200., true, false);
	const Region hsr = rotate(vsr, M_PI * 0.5);
	const Region crossr = [=](Point p) { return vsr(p) || hsr(p); };
	const Transformation polar_inverse = [=](Point p) {
		double r = to_polar(p).first;
		double phi = to_polar(p).second;
		return from_polar(Point(1000. / (1. + r * 0.02137), phi, true));
	};

	create_BMP("motylek_OwO.bmp", width, height, cond(compose(polar_inverse, crossr),
		constant(Color(0x3210fe)), constant(Color(0xdcba98))));


	std::vector<Vector> Vector_vector{
		Vector(-500, 150), Vector(21.37, 256), Vector(560, 71), Vector(123, -161), Vector(-142, -250)
	};
	std::vector<Color> mniej_fajna_nazwa{
		Color(0x765432), Color(0x10fedc), Color(0xba9876), Color(0x543210), Color(0x2137dd)
	};
	Image blobs = constant(Colors::white);
	const Blend cb2 = [](Point p){ return 1. / (1. + distance(p) / 200.); };
	for (int i = 0; i < 5; i++) {
		double tx = Vector_vector[i].first;
		double ty = Vector_vector[i].second;
		Transformation translation = [=](Point p){ return Point(p.first - tx, p.second - ty); };
		blobs = lerp(compose(translation, cb2), blobs, constant(mniej_fajna_nazwa[i]));
	}

	create_BMP("blobs.bmp", width, height, blobs);


	create_BMP("zing-zong_bing-bong_total_abomination.bmp", 1280, 720,
		scale<Color>(compose(twirl, swirl, lerp(compose(twirl, srb), cond(xorb, blobs, vs), lerp(cb, chck, pchck))), 0.75));
}
