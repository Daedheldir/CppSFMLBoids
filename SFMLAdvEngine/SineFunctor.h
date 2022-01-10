#pragma once

#include <iostream>
#include "FunctorBase.h"

class SineFunctor : public FunctorBase {
public:
	SineFunctor() : FunctorBase{ nullptr } {};
	const std::string var = "SineFunctor";

	virtual float operator () (float val1, float val2) const {
		float output = std::sinf((1 + val1) / (1 + val2)) * 255;
		return output;
	}
};
