#pragma once

#include <iostream>
#include "FunctorBase.h"
#include "PerlinNoise.hpp"

class PerlinNoiseFunctor : public FunctorBase {
public:

	PerlinNoiseFunctor() : FunctorBase{ nullptr }, perlin{ 0 } {};
	virtual std::string GetName() { return "PerlF"; };

	virtual float operator () (float val1, float val2) const {
		const float noise = static_cast<float>(255 * perlin.octave2D_01((val1 * 0.001), (val2 * 0.001), 4));

		return noise;
	}

	const siv::PerlinNoise perlin;
};
