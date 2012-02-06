/*
 * BlueNoise.hpp
 *
 *  Created on: Feb 6, 2012
 *      Author: david
 */

#ifndef BLUENOISE_HPP_
#define BLUENOISE_HPP_
//----------------------------------------------------------------------------//
#include <Slimage/Slimage.hpp>
#include <Danvil/Tools/FunctionCache.h>
#include <vector>
#include <cmath>
//----------------------------------------------------------------------------//
namespace dasp {
//----------------------------------------------------------------------------//

namespace BlueNoise
{
	// need to change some other functions too!!!
	constexpr unsigned int D = 2;

	struct Point {
		float x, y;
		float weight;
		float scale;
	};

	constexpr float KernelRange = 3.5f;

	constexpr float cMaxRefinementScale = 10.0f;

	inline
	float KernelFunctorImpl(float d) {
		constexpr float A = 0.39894228f;
		return A * std::exp(-0.5f*d*d);
	}

	inline
	float KernelFunctor(float d) {
		static Danvil::FunctionCache<float,1> cache(0.0f, KernelRange, &KernelFunctorImpl);
		return cache(std::abs(d));
	}

	inline
	float KernelFunctorSquareImpl(float d) {
		constexpr float A = 0.39894228f;
		return A * std::exp(-0.5f*d);
	}

	inline
	float KernelFunctorSquare(float d) {
		static Danvil::FunctionCache<float,1> cache(0.0f, KernelRange*KernelRange, &KernelFunctorSquareImpl);
		return cache(d);
	}

	inline
	float ZeroBorderAccess(const slimage::Image1f& density, int x, int y) {
		if(0 <= x && x < int(density.width()) && 0 <= y && y < int(density.height())) {
			return density(x, y);
		}
		else {
			return 0.0f;
		}
	}

	inline
	float KernelScaleFunction(float roh, float weight) {
//		return std::pow(roh / weight, -1.0f / float(D));
		return 1.0f / std::sqrt(roh / weight);
	}

	inline
	float ScalePowerD(float s) {
		//return std::pow(s, -float(D));
		return 1.0f / (s*s);
	}

	float EnergyApproximation(const std::vector<Point>& pnts, float x, float y);

	float Energy(const std::vector<Point>& pnts, const slimage::Image1f& density);

	void EnergyDerivative(const std::vector<Point>& pnts, const slimage::Image1f& density, unsigned int i, float& result_dE_x, float& result_dE_y);

	std::vector<Point> PlacePoints(const slimage::Image1f& density, unsigned int p);

	void Refine(std::vector<Point>& points, const slimage::Image1f& density, unsigned int iterations);

	std::vector<Point> Split(const std::vector<Point>& points, const slimage::Image1f& density, bool& result_added);

	std::vector<Point> Compute(const slimage::Image1f& density);

}

//----------------------------------------------------------------------------//
}
//----------------------------------------------------------------------------//
#endif
