#pragma once

#include <iostream>
#include "FunctorBase.h"

class SubtractionFunctor : public FunctorBase {
public:
	SubtractionFunctor() : FunctorBase{ nullptr } {};
	virtual std::string GetName() { return "SubF"; };

	virtual float operator () (float val1, float val2) const {
		return val1 - val2;
	}
};
