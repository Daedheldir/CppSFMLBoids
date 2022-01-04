#pragma once

#include <iostream>
#include "FunctorBase.h"

class SineFunctor : public FunctorBase {
public:
	SineFunctor() : FunctorBase{ nullptr } {};
	const std::string var = "SineFunctor";

	virtual float operator () (float val1, float val2) const {
		return std::sinf(val1) * val2;
	}
};
#pragma once
