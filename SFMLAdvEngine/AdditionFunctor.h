#pragma once

#include <iostream>
#include "FunctorBase.h"

class AdditionFunctor : public FunctorBase {
public:
	AdditionFunctor() {};
	AdditionFunctor(float val2) :FunctorBase{ val2 } {};
	const std::string var = "AdditionFunctor";

	virtual float operator () (float val1, float val2) {
		return val1 + val2;
	}
};
