#pragma once

#include <iostream>
#include "FunctorBase.h"

class SubtractionFunctor : public FunctorBase {
public:
	SubtractionFunctor() : FunctorBase{ nullptr } {};
	const std::string var = "Subtraction Functor";

	virtual float operator () (float val1, float val2) const {
		return val1 - val2;
	}
};
