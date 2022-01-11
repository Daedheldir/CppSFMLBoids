#pragma once

#include <iostream>
#include "FunctorBase.h"

class AdditionFunctor : public FunctorBase {
public:
	AdditionFunctor() : FunctorBase{ nullptr } {};
	virtual std::string GetName() { return "AddF"; };

	virtual float operator () (float val1, float val2) const {
		return val1 + val2;
	}
};
