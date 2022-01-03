#pragma once

#include <iostream>
#include "FunctorBase.h"

class AdditionFunctor : public FunctorBase {
public:
	AdditionFunctor() : FunctorBase{ nullptr } {};
	const std::string var = "AdditionFunctor";

	virtual float operator () (float val1, float val2) const {
		return val1 + val2;
	}
};
